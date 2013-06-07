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

		Matrix4x4* GetTexScaleBiasMat() {return &m_TexScaleBiasMat;}

	public:
		Matrix4x4	m_TexScaleBiasMat;
		Matrix4x4	m_TexMat;

		Matrix4x4	m_viewMat;
		Matrix4x4	m_projMat;

		int			m_nSizeX;
		int			m_nSizeY;

		Light*		m_pLight; 
		
		Camera*		m_pCamera;

		int			m_nIndex;

		Frustum		m_frustum;
	};
}


#endif
