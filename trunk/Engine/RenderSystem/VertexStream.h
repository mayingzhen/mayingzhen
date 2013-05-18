#ifndef  _VertexStream__H__
#define  _VertexStream__H__



namespace ma
{	
	class VertexDeclaration;
	class VertexBuffer;
	class IndexBuffer;

	class ENGINE_API VertexStream : public RefCountObject
	{
	public:	
// 		VertexDeclarationPtr	m_pDeclaration;
// 		VertexBufferPtr			m_pVertexBuffers[MAX_VERTEX_STREAM];
// 		IndexBufferPtr			m_pIndexBuffer;
		PRIMITIVE_TYPE			m_ePrimitiveType;
		VertexDeclaration*		m_pDeclaration;
 		VertexBuffer*			m_pVertexBuffers;
 		IndexBuffer*			m_pIndexBuffer;
	};

}

#endif
