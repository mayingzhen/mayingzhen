
#include "Blackboard.h"

namespace ma 
{

FrameGraphHandle Blackboard::getHandle(std::string const& name) const noexcept 
{
    auto it = mMap.find(name);
    if (it != mMap.end()) 
	{
        return it->second;
    }
    return {};
}

void Blackboard::remove(std::string const& name) noexcept
{
    mMap.erase(name);
}

} // namespace ma
