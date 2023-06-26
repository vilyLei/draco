#pragma once
#include <memory>
#include <vector>
#include "ctmDef.h"
#include "ctmUtils.h"
#include "draco/stream/DataView.h"
#include "draco/stream/Float32Array.h"
#include "draco/stream/Uint32Array.h"
#include "./FileHeader.h"

namespace draco
{
class CTMFileBody
{
public:
    CTMFileBody(std::shared_ptr<FileHeader> header) noexcept;
    virtual ~CTMFileBody();

    std::shared_ptr<draco::Uint32Array>  indices;
    std::shared_ptr<draco::Float32Array> vertices;
    std::shared_ptr<draco::Float32Array> normals;
    std::vector<UVMapType>                     uvMaps;
    std::vector<AttrMapType>                   attrMaps;

    std::shared_ptr<draco::Float32Array> getUVSAt(Int32 i);
};
}