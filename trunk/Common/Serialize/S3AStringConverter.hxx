
#include <Animation/Common/S3AStringConverter.h>

std::string S3AStringConverter::ToString(maGUID val)
{
	char buf[256];
	sprintf(buf,"(0x%lld,0x%lld)",val.m_a,val.m_b);
	return buf;
}
