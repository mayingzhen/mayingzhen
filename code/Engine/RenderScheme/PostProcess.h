#pragma once

#include "RenderStep.h"

namespace ma
{
	class RenderPass;
	class PostProcess;
	class PostProcessPipeline;
	class MainRenderView;

	class PostProcessStep : public RenderStep
	{
	public:
		PostProcessStep(PostProcess* pParent);

		virtual void PrepareRender(RenderProxy* proxy) override;

		virtual void Render() override;

		void Setup(RenderPass* pInFB, RenderPass* pOutFB);

		void SetInput(const char* pszSamplerName, const char* pszRenderTargetName);

		void SetOutput(const char* pszName);

		void SetTechnique(const char* pszTech);

		SubMaterial* GetMaterial() { return m_pMaterial.get(); }

		RenderPass* GetRenderPass() { return m_pRenderPass.get(); }

		void Import(rapidxml::xml_node<>* pXmlElem);
		void Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc);

		const char* GetName();
		void		SetName(const char* pszName);

	protected:
		std::string m_strName;

		std::string m_strTargetName;
		RefPtr<RenderPass> m_pRenderPass;

		std::map<std::string,std::string> m_strInTexture;

		RefPtr<SubMaterial> m_pMaterial;

		PostProcess* m_pParent = nullptr;
	};

	class PostProcess : public Referenced
	{
	public:
		PostProcess(PostProcessPipeline* pParent);

		void		Setup(RenderPass* pInFB,RenderPass* pOutFB);

		void		AddStep(PostProcessStep* pStep);

		PostProcessStep* GetStep(const char* pszName);

		void		Import(rapidxml::xml_node<>* pXmlElem);
		void		Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc);

		const char* GetName();
		void		SetName(const char* pszName);

		PostProcessPipeline* GetParent() { return m_pParent; }

		PostProcessStep* GetLastStep() { return m_vecStep.back().get(); }

	private:
		std::vector< RefPtr<PostProcessStep> > m_vecStep;

		std::string m_strName;

		bool m_bActive = true;

		PostProcessPipeline* m_pParent = nullptr;
	};

	class PostProcessPipeline : public Serializable
	{
	public:
		PostProcessPipeline(MainRenderView* view);

		void		Setup(RenderPass* pInFB, RenderPass* pOutFB);

		RenderPass* GetRenderPass(const char* pszName);

		Texture*	GetRenderTarget(const char* pszName);

		void		AddRenderPass(const char* pszName, int nWidth, int nHeight, PixelFormat format, bool bSRGB = false);
		void		AddRenderPass(const char* pszName, float fScale, PixelFormat format, bool bSRGB = false);

		void		AddPostProcess(PostProcess* pPostProcess);
		PostProcess* GetPostProcess(const char* pszName);

		bool		Import(rapidxml::xml_node<>* pXmlElem);
		bool		Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc);

		MainRenderView* GetMainRenderView() { return m_pView; }

		PostProcessStep* GetLastStep() { return m_vecPostProcess.back()->GetLastStep(); }

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
			bool m_sRGB = false;
			RefPtr<RenderPass> m_pRenderPass;
		};
		std::vector<TextureInfo> m_vecTextureInfo;

		RefPtr<RenderPass> m_pTempRenderPass;

		MainRenderView* m_pView = nullptr;
	};

}

