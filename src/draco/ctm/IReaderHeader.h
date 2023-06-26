#pragma once
#include <memory>
#include "ICTMRStream.h"
#include "CTMFileBody.h"

namespace draco
{
class IReaderHeader
{
public:
    virtual void read(std::shared_ptr<ICTMRStream> pstream, std::shared_ptr<CTMFileBody> pbody) = 0;
};
}