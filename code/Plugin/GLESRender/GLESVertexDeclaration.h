#ifndef  _GLESVertexDeclaration__H__
#define  _GLESVertexDeclaration__H__

namespace ma
{
	class GLESVertexDeclaration : public VertexDeclaration
	{

	public: 
		GLESVertexDeclaration();

		~GLESVertexDeclaration();

		virtual void RT_StreamComplete();
	};

}

#endif
