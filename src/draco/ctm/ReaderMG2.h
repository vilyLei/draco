#pragma once
#include <memory>
#include "IReaderHeader.h"
#include "FileMG2Header.h"
#include "draco/stream/Uint32Array.h"
#include "IDecoder.h"

namespace draco
{
class ReaderMG2 : public IReaderHeader
{
public:
    ReaderMG2();
    ~ReaderMG2();

    std::shared_ptr<FileMG2Header> MG2Header;

    void readAttrMaps(std::shared_ptr<ICTMRStream> stream, std::vector<AttrMapType>& attrMaps);
    void readUVMaps(std::shared_ptr<ICTMRStream> stream, std::vector<UVMapType>& uvMaps);

    void readNormals(std::shared_ptr<ICTMRStream> stream, std::shared_ptr<CTMFileBody> body);

    void readVertices(std::shared_ptr<ICTMRStream> stream, std::shared_ptr<draco::Float32Array> vertices);
    std::shared_ptr<draco::Uint32Array> readGridIndices(std::shared_ptr<ICTMRStream> stream, std::shared_ptr<draco::Float32Array> vertices);

    void readIndices(std::shared_ptr<ICTMRStream> stream, std::shared_ptr<draco::Int32Array> indices);
    void read(std::shared_ptr<ICTMRStream> stream, std::shared_ptr<CTMFileBody> body) override;

private:
};
}