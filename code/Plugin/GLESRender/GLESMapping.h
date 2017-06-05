#ifndef  _GLESMapping__H__
#define  _GLESMapping__H__


namespace ma
{
	class GLESMapping
	{
	public:
		static GLenum GetGLESTextureUSage(TEXTURE_USAGE Usage);
		static GLenum GetGLESBufferUSage(HBU_USAGE Usage);
 		static GLenum GetGLESPrimitiveType(PRIMITIVE_TYPE type);
		static GLenum GetGLESIndexType(INDEX_TYPE type);
		static void	  GetGLESDeclType(DECL_USAGE Usage, short nUsageIndex, DECL_TYPE eType, GLenum& type, 
			GLint& typeCount,GLboolean& normalized,std::string& name);
		static void	  GetGLESFilter(Filter type, GLenum& minFilter, GLenum& magFilter);
		static GLenum GetGLESWrap(Wrap type);
		static GLenum GetGLESFormat(PixelFormat format);
		static GLenum GetGLESDataType(PixelFormat mFormat);

		static GLint convertCompareFunction(CompareFunction func);
		static GLint convertStencilOp(StencilOperation op, bool invert = false);

		static PixelFormat GetClosestFormat(GLenum fmt, GLenum dataType);
	};

} 

#endif
