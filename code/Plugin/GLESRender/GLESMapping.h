#ifndef  _GLESMapping__H__
#define  _GLESMapping__H__


namespace ma
{
	class GLESMapping
	{
	public:
		static GLenum GetGLESUSage(USAGE Usage);
 		static GLenum GetGLESPrimitiveType(PRIMITIVE_TYPE type);
		static GLenum GetGLESIndexType(INDEX_TYPE type);
		static void	  GetGLESDeclType(DECL_USAGE Usage, GLenum& type, 
			GLint& typeCount,GLboolean& normalized,std::string& name);
		static void	  GetGLESFilter(FilterOptions type, GLenum& minFilter, GLenum& magFilter);
		static GLenum GetGLESWrap(Wrap type);
		static GLenum GetGLESFormat(FORMAT format);
	};

} 

#endif
