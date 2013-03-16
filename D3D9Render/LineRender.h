#ifndef  _LineRender__H__
#define  _LineRender__H__

namespace ma
{
	struct D3D9RENDER_API LineVertex
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR   col;
	};

	enum {LINE_BATCH_MAX = 2048};

	class D3D9RENDER_API LineRender
	{
	public:
		LineRender();

		void Init(LPDIRECT3DDEVICE9 pDxDevice);

		void DrawLine(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,xmUint32 dwColor);

		void FlushLine();

	private:
		ID3DXEffect*					m_pLineShader;

		IDirect3DVertexDeclaration9*	m_pPrimitiveVBDesc;

		LineVertex						m_LineBatch[LINE_BATCH_MAX*2];
		
		int								m_LineBatchSize;

		LPDIRECT3DDEVICE9				m_pDxDevice;
	};
}

#endif
