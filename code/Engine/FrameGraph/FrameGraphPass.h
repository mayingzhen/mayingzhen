#pragma once

#include <stdint.h>

namespace ma
{

class FrameGraphPassResources;

class FrameGraphPassExecutor 
{
    friend class FrameGraph;
    virtual void execute(FrameGraphPassResources const& resources) noexcept = 0;
public:
    FrameGraphPassExecutor();
    virtual ~FrameGraphPassExecutor();
    FrameGraphPassExecutor(FrameGraphPassExecutor const&) = delete;
    FrameGraphPassExecutor& operator = (FrameGraphPassExecutor const&) = delete;
};

template <typename Data, typename Execute>
class FrameGraphPass final : private FrameGraphPassExecutor 
{
    friend class FrameGraph;

    explicit FrameGraphPass(Execute&& execute) noexcept
            : FrameGraphPassExecutor(), mExecute(std::move(execute))
	{
    }
    void execute(FrameGraphPassResources const& resources) noexcept final 
	{
        mExecute(resources, mData);
    }
    Execute mExecute;
    Data mData;

public:
    Data const& getData() const noexcept { return mData; }
    Data& getData() noexcept { return mData; }
};

} // namespace ma

