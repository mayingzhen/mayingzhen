#ifndef  _RenderQueue__H__
#define  _RenderQueue__H__



namespace ma
{
	class UnitSphere;
	class ScreenQuad;
	class Technique;
	class Sampler;
	class Light;
	class ShadowMapFrustum;

	class ENGINE_API  RenderQueue
	{
	public:
		static void Init();

		static void AddRenderable(Renderable* pRenderable,bool bTrans = false);
		
		static void Clear();

		static void Fulsh();

		static void SetCamera(Camera* pCamera);

	private:
		static void ShadowDepthPass();
		static void ShadowDepthPass(Light* pLight);
			
		static void DeferredRending();
		static void GBufferPass();
		static void DeferredShadow();
		static void DefferedLighting();
		static void DefferedShading();

		static void ForwarRending();

	protected:
		static std::vector<Renderable*>   m_SolidEntry;
		static std::vector<Renderable*>   m_TransEntry;
		
		static RenderTarget*	m_pShadowTex;	

		static RenderTarget*	m_pDepthTex;
		static RenderTarget*	m_pNormalTex;

		static RenderTarget*	m_pDiffuse;
		static RenderTarget*	m_pSpecular;

		static Sampler*			m_pDiffuseTex;
		static Sampler*			m_pSpecularTex;


		static ScreenQuad*		m_pScreenQuad;

		static UnitSphere*		m_pUnitSphere;

		static Material*		m_pMaterDeferred;
		static Technique*		m_pTechGBuffer;
		static Technique*		m_pTechShading;

		static std::vector<Light*>	m_arrLight;

		static Light*				m_pSun;

		static Camera*				m_pCamera;

		static ShadowMapFrustum*	m_arrSMF[4];


		static	bool m_bUseDeferred;
	};

}

#endif