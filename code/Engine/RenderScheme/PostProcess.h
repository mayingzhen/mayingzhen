#pragma once

namespace ma
{
	class RenderPass;

	class PostProcessStep : public Referenced
	{
	public:
		void Setup(RenderPass* pInFB, RenderPass* pOutFB);

		void Render();

		void SetInput(const char* pszSamplerName, const char* pszRenderTargetName);

		void SetOutput(const char* pszName);

		void SetTechnique(const char* pszTech);

		RenderPass* GetRenderPass() { return m_pRenderPass.get(); }

		void Import(rapidxml::xml_node<>* pXmlElem, Resource* pParent);
		void Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc);

	protected:
		std::string m_strName;

		std::string m_strTargetName;
		RefPtr<RenderPass> m_pRenderPass;

		std::map<std::string,std::string> m_strInTexture;

		std::string m_strTech;
		RefPtr<SubMaterial> m_pMaterial;
	};

	class PostProcess : public Referenced
	{
	public:
		void	Setup(RenderPass* pInFB,RenderPass* pOutFB);

		void	Render();

		void	AddStep(PostProcessStep* pStep);

		void	Import(rapidxml::xml_node<>* pXmlElem);
		void	Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc);

	private:
		std::vector< RefPtr<PostProcessStep> > m_vecStep;

		std::string m_strName;

		bool m_bActive = true;
	};

	class PostProcessPipeline : public Serializable
	{
	public:
		void		Setup(RenderPass* pInFB, RenderPass* pOutFB);

		void		Render();

		RenderPass* GetRenderPass(const char* pszName);

		Texture*	GetRenderTarget(const char* pszName);

		void		AddRenderPass(const char* pszName, RenderPass* pRenderPass);
		void		AddRenderPass(const char* pszName, int nWidth, int nHeight, PixelFormat format, bool bSRGB = false);
		void		AddRenderPass(const char* pszName, float fScale, PixelFormat format, bool bSRGB = false);

		void		AddPostProcess(PostProcess* pPostProcess);

		bool		Import(rapidxml::xml_node<>* pXmlElem);
		bool		Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc);

	private:

		typedef std::vector< RefPtr<PostProcess> > VEC_POSTPROCESS;
		VEC_POSTPROCESS m_vecPostProcess;

		struct TextureInfo
		{
			std::string m_strName;
			std::string m_strFormat;
			float m_fScale = 0.0f;
			int m_nWidth;
			int m_nHight;
		};
		std::vector<TextureInfo> m_vecTextureInfo;

		typedef std::map<std::string, RefPtr<RenderPass> > MAP_RENDERPASS;
		MAP_RENDERPASS m_mapRenderPass;

		RefPtr<RenderPass> m_pTempRenderPass;
	};

	extern PostProcessPipeline* g_pPostProcessPipeline;
}

