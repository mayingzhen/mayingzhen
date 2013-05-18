#ifndef  _GLESVertexDeclaration__H__
#define  _GLESVertexDeclaration__H__

#include "GLESMapping.h"

namespace ma
{
	class Effect;
	class GLESRENDER_API GLESVertexDeclaration : public VertexDeclaration
	{

	public: 
		GLESVertexDeclaration();
		~GLESVertexDeclaration();

		virtual void Create() {}

		void Active(UINT nVertexStart,Effect* pEffect);

	protected:

	};

}

#endif
