#pragma once


#include "FrameGraphHandle.h"


namespace ma 
{

class Blackboard 
{
    using Container = std::map<std::string, FrameGraphHandle>;

public:
    auto& operator [](std::string const& name) noexcept 
	{
        return mMap.insert_or_assign(name, FrameGraphHandle{}).first;
    }

    template<typename T>
    void put(std::string const& name, FrameGraphId<T> handle) noexcept
	{
        mMap.insert_or_assign(name, handle);
    }

    template<typename T>
    FrameGraphId<T> get(std::string&& name) const noexcept
	{
        return static_cast<FrameGraphId<T>>(getHandle(std::forward<std::string>(name)));
    }

    void remove(std::string const& name) noexcept;

private:
    FrameGraphHandle getHandle(std::string const& name) const noexcept;
    Container mMap;
};

} // namespace ma

