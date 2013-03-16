#ifndef  _ShadowMap__H__
#define  _ShadowMap__H__

namespace ma
{
	class ShadowMap
	{
	public:
		ShadowMap();
		
		virtual ~ShadowMap();

		virtual bool Create(int iSizeX,int iSizeY);

		void Destroy();

		void BeginRender();

		void EndRender();

		D3DXMATRIX* GetTexScaleBiasMat() {return &m_TexScaleBiasMat;}

	public:
		D3DXMATRIX	m_TexScaleBiasMat;
		D3DXMATRIX	m_TexMat;

		D3DXMATRIX	m_viewMat;
		D3DXMATRIX	m_projMat;

		int			m_nSizeX;
		int			m_nSizeY;

		Light*		m_pLight; 
		
		Camera*		m_pCamera;

		int			m_nIndex;

		Frustum		m_frustum;
	};
}


#endif
