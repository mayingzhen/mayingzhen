#ifndef  _IndexBuffer__H__
#define  _IndexBuffer__H__

#include "HardwareBuffer.h"

namespace ma
{
	class RENDER_API IndexBuffer : public HardwareBuffer
	{
	public:
		IndexBuffer()
		{
		}

		INDEX_TYPE GetIndexType() {return m_Stride == 2 ? INDEX_TYPE_U16 : INDEX_TYPE_U32;}
	
	//protected:
	//	INDEX_TYPE				m_eIndexType;
	};
}

#endif