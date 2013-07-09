#ifndef  _VertexDeclaration__H__
#define  _VertexDeclaration__H__

#include "RenderDefine.h"

namespace ma
{

	class ENGINE_API VertexDeclaration : public RefCountObject
	{
	public: 
		VertexDeclaration();

		virtual ~VertexDeclaration();
			
		virtual void	Active() = 0;

		void			Init(UINT nVertexType);

		void			AddElement(short number, short Offset, DECL_TYPE DeclType, DECL_USAGE DeclUsage, unsigned char index);

		int				GetStreanmStride() const {return m_nStreamStride;} 
		
		int				GetElementStride(int index) ;
		
		int				GetElementCount() { return m_ElementCount; }
		
		const VertexElement& GetElement(int index) { ASSERT (index < m_ElementCount); return m_Elements[index]; }

	protected:
		VertexElement	m_Elements[MAX_ELEMENT];
		int				m_ElementCount;
		int				m_nStreamStride;

	};

	DeclareSmartPtr(VertexDeclaration);

}

#endif