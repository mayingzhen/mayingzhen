#pragma once
#include "ImageCodec.h"

namespace ma
{
	ENGINE_API void PVRTCDecompress(IN OUT ImageData& imgData);
	ENGINE_API void ETC1Decompress(IN OUT ImageData& imgData);
	ENGINE_API void DXTDecompress(IN OUT ImageData& imageData);
}