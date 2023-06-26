#pragma once
#include "baseDef.h"

namespace draco
{
class ArrayBuffer
{
public:
    ArrayBuffer(Int32 byteLength);
    ArrayBuffer(Int8* bytes, Int32 bytesTotal);
    ArrayBuffer(char* bytes, Int32 bytesTotal);
    virtual ~ArrayBuffer();

    Int32 getByteLength();
    Int8*  getBytes();

private:
    Int32 m_byteLength;
    Int8  m_buildType;
    Int8* m_bytes;
};

} // namespace base