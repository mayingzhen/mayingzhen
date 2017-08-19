#pragma once

#ifdef _MSC_VER

	#ifdef D3D11RENDER_EXPORTS
	#define D3D11RENDER_API _declspec (dllexport)
	#else
	#define D3D11RENDER_API _declspec (dllimport)
	#endif

#else

	#define D3D11RENDER_API

#endif
