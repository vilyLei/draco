#pragma once
#include <vector>
#include "ctmUtils.h"
#include "IReaderHeader.h"
#include "draco/stream/Int32Array.h"
#include "CTMFileBody.h"

namespace draco
{
class ReaderRAW : public IReaderHeader
{
public:
    ReaderRAW()  = default;
    ~ReaderRAW() = default;

    void readAttrMaps(std::shared_ptr<ICTMRStream> stream, std::vector<AttrMapType>& attrMaps);
    void readUVMaps(std::shared_ptr<ICTMRStream> stream, std::vector<UVMapType>& uvMaps);
    void readNormals(std::shared_ptr<ICTMRStream> stream, std::shared_ptr<draco::Float32Array> normals);

    void readVertices(std::shared_ptr<ICTMRStream> stream, std::shared_ptr<draco::Float32Array> vertices);
    void readIndices(std::shared_ptr<ICTMRStream> stream, std::shared_ptr<draco::Int32Array> indices);
    void read(std::shared_ptr<ICTMRStream> stream, std::shared_ptr<CTMFileBody> body) override;
};
}