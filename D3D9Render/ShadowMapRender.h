#ifndef  _ShadowMapRender__H__
#define  _ShadowMapRender__H__

namespace ma
{


	class D3D9RENDER_API ShdowMapRender
	{
	public:
		ShdowMapRender();

		void Init(LPDIRECT3DDEVICE9 pDxDevice);

		void CalculateShadowMap(Light* pLigt,Camera* pCamera);

	private:
		LPDIRECT3DDEVICE9 m_pDxDevice;

		enum {MAX_SHDOWMAP = 4};
		ShadowMap* m_arrShadowMap[MAX_SHDOWMAP];
	}
}

#endif

