#ifndef  _GLESVertexDeclaration__H__
#define  _GLESVertexDeclaration__H__

namespace ma
{
	class GLESRENDER_API GLESVertexDeclaration : public VertexDeclaration
	{

	public: 
		GLESVertexDeclaration();

		~GLESVertexDeclaration();

		virtual void Active();
	};

}

#endif
