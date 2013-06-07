#ifndef  _GLESVertexDeclaration__H__
#define  _GLESVertexDeclaration__H__

#include "GLESMapping.h"

namespace ma
{
	class ShaderProgram;
	class GLESRENDER_API GLESVertexDeclaration : public VertexDeclaration
	{

	public: 
		GLESVertexDeclaration();

		~GLESVertexDeclaration();

		virtual void Active() {}

		//void Bind(UINT nVertexStart,ShaderProgram* pEffect);

		//void UnBind();

	protected:
		
	};

}

#endif
