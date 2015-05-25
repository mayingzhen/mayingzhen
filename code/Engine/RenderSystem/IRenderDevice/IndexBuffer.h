#ifndef  _IndexBuffer__H__
#define  _IndexBuffer__H__

#include "HardwareBuffer.h"

namespace ma
{
	class IndexBuffer : public HardwareBuffer
	{
	public:
		IndexBuffer()
		{
		}

		INDEX_TYPE GetIndexType() {return m_Stride == 2 ? INDEX_TYPE_U16 : INDEX_TYPE_U32;}
	
	};

}

#endif