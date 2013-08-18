#ifndef  _GLESMapping__H__
#define  _GLESMapping__H__


#include "Engine/RenderSystem/RenderDefine.h"


namespace ma
{

	struct GLESRENDER_API GLESRenderState
	{
		DWORD       CullMode;
		DWORD       FillMode;
		DWORD       ShadeMode;
		DWORD       ColorWrite;
		DWORD       DepthWrite;
		DWORD       DepthCheck;
		DWORD       DepthFunc;
		DWORD       SrcBlend;
		DWORD       DestBlend;
		DWORD       BlendOp;
		DWORD       AlphaTest;
		DWORD       AlphaFunc;
		DWORD       AlphaRef;
	};

	struct  GLESDisplayDevice
	{

	};

	class GLESMapping
	{
	public:
		static GLenum GetGLESUSage(USAGE Usage);
 		static GLenum GetGLESPrimitiveType(PRIMITIVE_TYPE type);
		static GLenum GetGLESIndexType(INDEX_TYPE type);
		static void	  GetGLESDeclType(DECL_USAGE Usage, GLenum& type, 
			GLint& typeCount,GLboolean& normalized,std::string& name);
		static void	  GetGLESFilter(Sampler::FilterOptions type, GLenum& minFilter, GLenum& magFilter);
		static GLenum GetGLESWrap(Sampler::Wrap type);
		static GLenum GetGLESFormat(FORMAT format);
	};

} 

#endif