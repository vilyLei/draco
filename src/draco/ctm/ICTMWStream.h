#pragma once
#include "ctmDef.h"
#include <memory>
#include "draco/stream/Uint8Array.h"


namespace draco
{
class ICTMWStream
{
public:
    /**
     * uint8 array
     */
    std::shared_ptr<draco::Uint8Array> data;
    Int32                                    offset;
    Int32                                    count;
    Int32                                    len;
    virtual void                             writeByte(Uint8 value) = 0;
};
}