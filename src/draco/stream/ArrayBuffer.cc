#include <cassert>
#include "ArrayBuffer.h"

namespace draco
{
ArrayBuffer::ArrayBuffer(Int32 byteLength) :
    m_byteLength(byteLength), m_buildType(0)
{
    assert(byteLength > 0);
    m_bytes = new Int8[byteLength]{};
    //std::cout << "ArrayBuffer::constructor()..." << byteLength << std::endl;
}

ArrayBuffer::ArrayBuffer(Int8* bytes, Int32 bytesTotal) :
    m_byteLength(bytesTotal), m_buildType(1), m_bytes(bytes)
{
    assert(bytesTotal > 0 && bytes != nullptr);
    //std::cout << "ArrayBuffer::constructor()..." << byteLength << std::endl;
}
ArrayBuffer::ArrayBuffer(char* bytes, Int32 bytesTotal) :
    m_byteLength(bytesTotal), m_buildType(1), m_bytes(reinterpret_cast<Int8*>(bytes))
{
    assert(bytesTotal > 0 && bytes != nullptr);
    //std::cout << "ArrayBuffer::constructor()..." << byteLength << std::endl;
}
ArrayBuffer::~ArrayBuffer()
{
    if (m_buildType < 1) {
        DELETE_OBJ_ARRAY_NAKED_PTR(m_bytes);
    }
    m_bytes = nullptr;
    //std::cout << "ArrayBuffer::deconstructor()..." << std::endl;
}

Int32 ArrayBuffer::getByteLength()
{
    return m_byteLength;
}
Int8* ArrayBuffer::getBytes()
{
    return m_bytes;
}

} // namespace base