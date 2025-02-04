#pragma once
#include <string>
#include <memory>
#include "draco/stream/ArrayBuffer.h"
#include "draco/stream/DataView.h"
#include "draco/stream/Uint8Array.h"
#include "draco/stream/Uint16Array.h"
#include "draco/stream/Float32Array.h"

namespace draco
{
template <typename intT, typename uintT>
intT rightMoveBitFillZero(intT v, unsigned int bitsSteps)
{
    auto t = static_cast<uintT>(v);
    t >>= bitsSteps;
    return static_cast<intT>(t);
}

Int32 static __ctmIsLittleEndian = 0;

bool static isLittleEndian()
{
    if (__ctmIsLittleEndian > 0)
    {
        return __ctmIsLittleEndian == 1;
    }
    auto buffer = std::make_shared<draco::ArrayBuffer>(2);

    draco::Uint8Array  bytes(buffer);
    draco::Uint16Array ints(buffer);

    bytes[0] = 1;

    __ctmIsLittleEndian = ints[0] == 1 ? 1 : 2;
    return __ctmIsLittleEndian == 1;
}

struct UVMapType
{
    UVMapType() = default;
    ~UVMapType()
    {
        //std::cout << "      UVMapType::deconstructor() ..." << std::endl;
        uv = nullptr;
    }
    std::string                                name;
    std::string                                filename;
    std::shared_ptr<draco::Float32Array> uv;
};

struct AttrMapType
{
    AttrMapType() = default;
    ~AttrMapType()
    {
        //std::cout << "      AttrMapType::deconstructor() ..." << std::endl;
        attr = nullptr;
    }
    std::string                            name;
    std::shared_ptr<draco::DataView> attr;
};

class CompressionMethod
{
public:
    static const Int32 RAW = 0x00574152;
    static const Int32 MG1 = 0x0031474d;
    static const Int32 MG2 = 0x0032474d;
};
enum class CTMFlags
{
    NORMALS = 0x00000001
};

template <typename T>
void showArrayViewPtr(T vs, std::string ns = "", int begin = 0, int end = 30)
{
    if (vs != nullptr)
    {
        if (end > vs->getLength())
        {
            end = vs->getLength();
        }
        std::string str;
        for (auto i = 0; i < end; ++i)
        {
            if ((i + 1) < end)
            {
                str = str + std::to_string((*vs)[i]) + ",";
            }
            else
            {
                str = str + std::to_string((*vs)[i]);
            }
        }
        std::cout << "      " << ns << "(" << (*vs).getLength() << "), value: " << str << std::endl;
    }
}

}