#include <Common/Serialize/StringConverter.h>

std::string StringConverter::ToString(maGUID val)
{
	char buf[256];
	sprintf(buf,"(0x%lld,0x%lld)",val.m_a,val.m_b);
	return buf;
}
