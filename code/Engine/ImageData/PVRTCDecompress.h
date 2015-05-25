#ifndef __PVRTCDecompress_H__
#define __PVRTCDecompress_H__

#include "ImageCodec.h"

namespace ma
{
	 void PVRTCDecompress(IN OUT ImageData& imgData);
	 void ETC1Decompress(IN OUT ImageData& imgData);
	 void DXTDecompress(IN OUT ImageData& imageData);
}

#endif