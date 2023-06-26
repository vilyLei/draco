// Copyright 2016 The Draco Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "draco/io/ctm_decoder.h"

#include <cctype>
#include <cmath>
#include <utility>

#include "draco/io/file_utils.h"
#include "draco/io/parser_utils.h"
#include "draco/metadata/geometry_metadata.h"

#include "draco/stream/ArrayBuffer.h"
#include "draco/ctm/CTMStream.h"
#include "draco/ctm/CTMFile.h"

namespace draco {

    CTMDecoder::CTMDecoder()
        : m_ctmFileU8Arr(nullptr),
        counting_mode_(true),
        num_ctm_faces_(0),
        num_positions_(0),
        num_tex_coords_(0),
        num_normals_(0),
        num_materials_(0),
        last_sub_obj_id_(0),
        pos_att_id_(-1),
        tex_att_id_(-1),
        norm_att_id_(-1),
        material_att_id_(-1),
        sub_obj_att_id_(-1),
        added_edge_att_id_(-1),
        deduplicate_input_values_(true),
        last_material_id_(0),
        use_metadata_(false),
        preserve_polygons_(false),
        has_polygons_(false),
        mesh_files_(nullptr),
        out_mesh_(nullptr),
        out_point_cloud_(nullptr) {}

    Status CTMDecoder::DecodeFromFile(const std::string& file_name,
        Mesh* out_mesh) {
        return DecodeFromFile(file_name, out_mesh, nullptr);
    }

    Status CTMDecoder::DecodeFromFile(const std::string& file_name, Mesh* out_mesh,
        std::vector<std::string>* mesh_files) {
        out_mesh_ = out_mesh;
        mesh_files_ = mesh_files;
        return DecodeFromFile(file_name, static_cast<PointCloud*>(out_mesh));
    }

    Status CTMDecoder::DecodeFromFile(const std::string& file_name,
        PointCloud* out_point_cloud) {
        //printf("DecodeFromFile() in draco/io/ctm_decoder.cc file.\n");
        std::vector<char> buffer;
        if (!ReadFileToBuffer(file_name, &buffer)) {
            return Status(Status::DRACO_ERROR, "Unable to read input file.");
        }


        std::shared_ptr<ArrayBuffer> arrBuf(new ArrayBuffer(reinterpret_cast<Int8*>(buffer.data()), buffer.size()));
        m_ctmFileU8Arr = std::make_shared<Uint8Array>(arrBuf);

        buffer_.Init(buffer.data(), buffer.size());

        out_point_cloud_ = out_point_cloud;
        input_file_name_ = file_name;
        return DecodeInternal();
    }

    Status CTMDecoder::DecodeFromBuffer(DecoderBuffer* buffer, Mesh* out_mesh) {
        out_mesh_ = out_mesh;
        return DecodeFromBuffer(buffer, static_cast<PointCloud*>(out_mesh));
    }

    Status CTMDecoder::DecodeFromBuffer(DecoderBuffer* buffer,
        PointCloud* out_point_cloud) {
        out_point_cloud_ = out_point_cloud;
        buffer_.Init(buffer->data_head(), buffer->remaining_size());
        return DecodeInternal();
    }

    Status CTMDecoder::DecodeInternal() {
        // In the first pass, count the number of different elements in the geometry.
        // In case the desired output is just a point cloud (i.e., when
        // out_mesh_ == nullptr) the decoder will ignore all information about the
        // connectivity that may be included in the source data.
        counting_mode_ = true;
        ResetCounters();
        material_name_to_id_.clear();
        last_sub_obj_id_ = 0;
        // Parse all lines.
        Status status(Status::OK);
        // m_ctmFileU8Arr

        std::cout << "      ------------- ctm model parse begin --------" << std::endl;

        auto stream = std::make_shared<CTMStream>(m_ctmFileU8Arr);
        auto ctmFile = std::make_shared<CTMFile>(stream);

        auto ctmBody = ctmFile->body;

        /*
        showArrayViewPtr(ctmBody->indices, "ctmBody->indices\n");
        if (ctmBody->normals) {
            showArrayViewPtr(ctmBody->normals, "ctmBody->normals\n");
        }
        if (ctmBody->uvMaps.size() > 0)
        {
            auto uvs = ctmBody->getUVSAt(0);
            showArrayViewPtr(uvs, "ctmBody->uvs\n");
        }
        showArrayViewPtr(ctmBody->vertices, "ctmBody->vertices\n");
        std::cout << "      ------------- ctm model parse end --------\n" << std::endl;
        //*/
        m_ctmFileU8Arr = nullptr;

        if (ctmBody->normals) {
            num_normals_ = ctmBody->normals->getLength() / 3;
        }
        if (ctmBody->uvMaps.size() > 0)
        {
            auto uvs = ctmBody->getUVSAt(0);
            num_tex_coords_ = uvs->getLength() / 2;
        }
        num_ctm_faces_ = ctmBody->indices->getLength() / 3;
        num_positions_ = ctmBody->vertices->getLength() / 3;

        if (num_ctm_faces_ < 1) {
            return status;
        }
        bool use_identity_mapping = false;


        // Initialize point cloud and mesh properties.
        if (out_mesh_) {
            // Start decoding a mesh with the given number of faces. For point clouds we
            // silently ignore all data about the mesh connectivity.
            out_mesh_->SetNumFaces(num_ctm_faces_);
        }
        out_point_cloud_->set_num_points(3 * num_ctm_faces_);
        // Add attributes if they are present in the input data.

        auto indicesLen = ctmBody->indices->getLength();
        auto indices = *(ctmBody->indices);

        if (num_positions_ > 0) {
            GeometryAttribute va;
            va.Init(GeometryAttribute::POSITION, nullptr, 3, DT_FLOAT32, false, sizeof(float) * 3, 0);
            pos_att_id_ = out_point_cloud_->AddAttribute(va, use_identity_mapping, num_positions_);

            float val[3];
            auto attr = out_point_cloud_->attribute(pos_att_id_);
            auto j = 0;
            auto vs = ctmBody->vertices;
            for (auto i = 0; i < num_positions_; ++i) {
                val[0] = (*vs)[j++];
                val[1] = (*vs)[j++];
                val[2] = (*vs)[j++];
                attr->SetAttributeValue(AttributeValueIndex(i), val);

            }

            for (auto i = 0; i < indicesLen; i++) {
                const PointIndex vert_id(i);
                attr->SetPointMapEntry(vert_id, AttributeValueIndex(indices[i]));
            }
        }


        if (num_normals_ > 0) {
            GeometryAttribute va;
            va.Init(GeometryAttribute::NORMAL, nullptr, 3, DT_FLOAT32, false, sizeof(float) * 3, 0);
            norm_att_id_ = out_point_cloud_->AddAttribute(va, use_identity_mapping, num_normals_);

            float val[3];
            auto attr = out_point_cloud_->attribute(norm_att_id_);
            auto j = 0;
            auto nvs = ctmBody->normals;

            for (auto i = 0; i < num_normals_; ++i) {
                val[0] = (*nvs)[j++];
                val[1] = (*nvs)[j++];
                val[2] = (*nvs)[j++];
                attr->SetAttributeValue(AttributeValueIndex(i), val);

            }

            for (auto i = 0; i < indicesLen; i++) {
                const PointIndex vert_id(i);
                attr->SetPointMapEntry(vert_id, AttributeValueIndex(indices[i]));
            }
        }

        if (num_tex_coords_ > 0) {
            GeometryAttribute va;
            va.Init(GeometryAttribute::TEX_COORD, nullptr, 2, DT_FLOAT32, false, sizeof(float) * 2, 0);
            tex_att_id_ = out_point_cloud_->AddAttribute(va, use_identity_mapping, num_tex_coords_);

            float val[2];
            auto attr = out_point_cloud_->attribute(tex_att_id_);
            auto j = 0;
            auto uvs = ctmBody->getUVSAt(0);
            
            for (auto i = 0; i < num_tex_coords_; ++i) {
                val[0] = (*uvs)[j++];
                val[1] = (*uvs)[j++];
                attr->SetAttributeValue(AttributeValueIndex(i), val);

            }

            for (auto i = 0; i < indicesLen; i++) {
                const PointIndex vert_id(i);
                attr->SetPointMapEntry(vert_id, AttributeValueIndex(indices[i]));
            }
        }


        if (out_mesh_) {
            // Add faces with identity mapping between vertex and corner indices.
            // Duplicate vertices will get removed later.
            Mesh::Face face;
            for (FaceIndex i(0); i < num_ctm_faces_; ++i) {
                for (int c = 0; c < 3; ++c) {
                    face[c] = 3 * i.value() + c;
                }
                out_mesh_->SetFace(i, face);
            }
            //Mesh::Face face;
            //int k = 0;
            //for (FaceIndex i(0); i < num_ctm_faces_; ++i) {
            //    for (int c = 0; c < 3; ++c) {
            //        face[c] = PointIndex(indices[k++]);
            //    }
            //    out_mesh_->SetFace(i, face);
            //}
            //Mesh::Face face;
            //int k = 0;
            //for (auto i = 0; i < indicesLen;) {
            //    //const PointIndex vert_id(i);
            //    //attr->SetPointMapEntry(vert_id, AttributeValueIndex(indices[i]));
            //    //k = indices[i];
            //    for (int c = 0; c < 3; ++c) {
            //        face[c] = PointIndex(indices[i]);
            //        i++;
            //    }
            //    out_mesh_->SetFace(FaceIndex(k++), face);
            //}
        }

#ifdef DRACO_ATTRIBUTE_VALUES_DEDUPLICATION_SUPPORTED
        if (deduplicate_input_values_) {
            out_point_cloud_->DeduplicateAttributeValues();
        }
#endif
#ifdef DRACO_ATTRIBUTE_INDICES_DEDUPLICATION_SUPPORTED
        out_point_cloud_->DeduplicatePointIds();
#endif
        return status;
    }

    void CTMDecoder::ResetCounters() {
        num_ctm_faces_ = 0;
        num_positions_ = 0;
        num_tex_coords_ = 0;
        num_normals_ = 0;
        last_material_id_ = 0;
        last_sub_obj_id_ = 0;
    }


    // Methods Triangulate() and IsNewEdge() are used for polygon triangulation and
    // representation as an attribute for reconstruction in the decoder.
    //
    // Polygon reconstruction attribute is associated with every triangle corner and
    // has values zero or one. Zero indicates that an edge opposite to the corner is
    // present in the original mesh (dashed lines), and one indicates that the
    // opposite edge has been added during polygon triangulation (dotted lines).
    //
    // Polygon triangulation is illustrated below. Pentagon ABCDE is split into
    // three triangles ABC, ACD, ADE. It is sufficient to set polygon reconstruction
    // attribute at corners ABC and ACD. The attribute at the second corner of all
    // triangles except for the last is set to one.
    //
    //          C           D
    //          * --------- *
    //         /. 1     0  .|
    //        / .         . |
    //       /  .        .  |
    //      / 0 .       . 0 |
    //     /    .      .    |
    //  B * 1   .     .     |
    //     \    .    .      |
    //      \ 0 . 0 .       |
    //       \  .  .        |
    //        \ . .         |
    //         \..  0     0 |
    //          *-----------*
    //          A           E
    //
    inline int CTMDecoder::Triangulate(int tri_index, int tri_corner) {
        return tri_corner == 0 ? 0 : tri_index + tri_corner;
    }

    inline bool CTMDecoder::IsNewEdge(int tri_count, int tri_index,
        int tri_corner) {
        // All but the last triangle of the triangulated polygon have an added edge
        // opposite of corner 1.
        return tri_index != tri_count - 1 && tri_corner == 1;
    }

}  // namespace draco
