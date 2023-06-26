#pragma once
#include <string>
#include <memory>
#include "ICTMRStream.h"
#include "draco/stream/Uint8Array.h"


namespace draco
{
class CTMStream : public ICTMRStream
{
public:
    CTMStream(std::shared_ptr<draco::Uint8Array> pdata);
    virtual ~CTMStream();

    void                                       showInfo() override;
    Uint8                                      readByte() override;
    Int32                                      readInt32() override;
    Float32                                    readFloat32() override;
    std::string                                readString() override;
    std::shared_ptr<draco::Int32Array>   readArrayInt32(std::shared_ptr<draco::Int32Array> arr) override;
    std::shared_ptr<draco::Float32Array> readArrayFloat32(std::shared_ptr<draco::Float32Array> arr) override;

    const static double TWO_POW_MINUS23;
    const static double TWO_POW_MINUS126;
private:
    std::string decodeUint8Arr(std::shared_ptr<draco::Uint8Array> pu8array);
};
}