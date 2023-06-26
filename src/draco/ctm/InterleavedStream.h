#pragma once
#include "ctmDef.h"
#include "ICTMWStream.h"
#include "draco/stream/DataView.h"

namespace draco
{
class InterleavedStream : public ICTMWStream
{

public:
    InterleavedStream(std::shared_ptr<draco::DataView> pdata, Int32 pcount) noexcept;
    virtual ~InterleavedStream() noexcept;

    void writeByte(Uint8 value) override;

    static bool CTM_isLittleEndian;
};
}