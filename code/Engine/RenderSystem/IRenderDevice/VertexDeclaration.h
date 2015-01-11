#ifndef  _VertexDeclaration__H__
#define  _VertexDeclaration__H__

#include "RenderDefine.h"

namespace ma
{

	class RENDER_API VertexDeclaration : public Referenced
	{
	public: 
		VertexDeclaration();

		virtual ~VertexDeclaration();
			
		virtual void			RT_StreamComplete() = 0;

		void					Init(UINT nVertexType);

		void					AddElement(short number, short Offset, DECL_TYPE DeclType, DECL_USAGE DeclUsage, unsigned char index);
		
		void					AddElement(const VertexElement& element);

		int						GetStreanmStride() const {return m_nStreamStride;} 
		
		int						GetElementCount() { return m_ElementCount; }
		
		const VertexElement&	GetElement(int index) { ASSERT (index < m_ElementCount); return m_Elements[index]; }

	private:
		int						GetDeclTypeSize(DECL_TYPE type);

	protected:
		VertexElement			m_Elements[MAX_ELEMENT];

		int						m_ElementCount;
		
		int						m_nStreamStride;
	};

	DeclareRefPtr(VertexDeclaration);
}

#endif