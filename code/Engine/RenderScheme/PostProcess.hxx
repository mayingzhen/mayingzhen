#include "PostProcess.h"

namespace ma
{
	void PostProcessStep::SetInput(const char* pszSamplerName, const char* pszRenderTargetName)
	{
		m_strInTexture[pszSamplerName] = pszRenderTargetName;
	}

	void PostProcessStep::SetOutput(const char* pszName)
	{
		m_strTargetName = pszName;
	}

	void PostProcessStep::SetTechnique(const char* pszTech)
	{
		m_strTech = pszTech;
	}

	void PostProcessStep::Setup(RenderPass* pInFB, RenderPass* pOutFB)
	{
		m_pRenderPass = g_pPostProcessPipeline->GetRenderPass(m_strTargetName.c_str());
		if (m_strTargetName == "[StageOutput]")
		{
			m_pRenderPass = pOutFB;
		}
		ASSERT(m_pRenderPass);
		
		RefPtr<Technique> pTech = CreateTechnique(m_strTech.c_str(), "", m_pRenderPass.get());
		m_pMaterial = CreateSubMaterial();
		m_pMaterial->SetShadingTechnqiue(pTech.get());

		for (auto it = m_strInTexture.begin(); it != m_strInTexture.end(); ++it)
		{
			Texture* pTex = nullptr;
			std::string strIn = it->second;
			if (strIn == "[StageInput]")
			{
				pTex = pInFB->m_arrColor[0].m_pTexture.get();
			}
			else
			{
				pTex = g_pPostProcessPipeline->GetRenderTarget(strIn.c_str());
			}
			ASSERT(pTex);
			
			RefPtr<SamplerState> pSampler = CreateSamplerState(pTex);
			m_pMaterial->SetParameter(it->first.c_str(), Any(pSampler));
		}
	}

	void PostProcessStep::Render()
	{
		m_pRenderPass->Begine();

		RefPtr<Technique> pTech = m_pMaterial->GetShadingTechnqiue();

		ScreenQuad::Render(pTech.get(), m_pRenderPass->GetThreadCommand(0, (RenderListType)0));

		m_pRenderPass->End();
	}

	void PostProcessStep::Import(rapidxml::xml_node<>* pXmlElem, Resource* pParent)
	{

	}

	void PostProcessStep::Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc)
	{

	}

	void PostProcess::AddStep(PostProcessStep* pStep)
	{
		m_vecStep.push_back(pStep);
	}

	void PostProcess::Setup(RenderPass* pInFB, RenderPass* pOutFB)
	{
		for (uint32_t i = 0; i < m_vecStep.size(); ++i)
		{
			m_vecStep[i]->Setup(pInFB, pOutFB);
		}
	}

	void PostProcess::Render()
	{
		if (!m_bActive)
			return;

		for (uint32_t i = 0; i < m_vecStep.size(); ++i)
		{
			m_vecStep[i]->Render();
		}
	}

	void PostProcessPipeline::Setup(RenderPass* pInFB, RenderPass* pOutFB)
	{
		if (m_vecPostProcess.empty())
			return;

		RenderPass* pPPInFB = pInFB;
		RenderPass* pPPOutFB = pOutFB;

		if (m_vecPostProcess.size() > 1)
		{
			pPPOutFB = m_pTempRenderPass.get();
		}

		for (uint32_t i = 0; i < m_vecPostProcess.size(); ++i)
		{
			m_vecPostProcess[i]->Setup(pPPInFB, pPPOutFB);
			
			std::swap(pPPInFB, pPPOutFB);

			if (i == m_vecPostProcess.size() - 1)
			{
				pPPOutFB = pOutFB;
			}
		}

	}

	void PostProcessPipeline::Render()
	{
		for (uint32_t i = 0; i < m_vecPostProcess.size(); ++i)
		{
			m_vecPostProcess[i]->Render();
		}
	}

	void PostProcessPipeline::AddRenderPass(const char* pszName, RenderPass* pRenderPass)
	{
		m_mapRenderPass[pszName] = pRenderPass;
	}

	void PostProcessPipeline::AddRenderPass(const char* pszName, float fScale, PixelFormat format, bool bSRGB)
	{
		Rectangle rect = GetRenderSystem()->GetViewPort();
		int nWidth = (int)(fScale * rect.width());
		int nHeight = (int)(fScale * rect.height());

		AddRenderPass(pszName, nWidth, nHeight, format, bSRGB);
	}

	void PostProcessPipeline::AddRenderPass(const char* pszName, int nWidth, int nHeight, PixelFormat format, bool bSRGB)
	{
		RefPtr<RenderPass> pRenderPass = GetRenderDevice()->CreateRenderPass();
		RefPtr<Texture> pTex = GetRenderSystem()->CreateRenderTarget(nWidth, nHeight, 1, format, bSRGB);
		pRenderPass->AttachColor(0, pTex.get(), 0, 0);
		GetRenderSystem()->RenderPassStreamComplete(pRenderPass.get());

		AddRenderPass(pszName, pRenderPass.get());
	}

	void PostProcessPipeline::AddPostProcess(PostProcess* pPostProcess)
	{
		m_vecPostProcess.push_back(pPostProcess);
	}

	RenderPass* PostProcessPipeline::GetRenderPass(const char* pszName)
	{
		auto it = m_mapRenderPass.find(pszName);
		if (it == m_mapRenderPass.end())
			return nullptr;

		return it->second.get();
	}

	Texture* PostProcessPipeline::GetRenderTarget(const char* pszName)
	{
		RenderPass* pRenderPass = GetRenderPass(pszName);
		if (pRenderPass == NULL)
			return NULL;

		return pRenderPass->m_arrColor[0].m_pTexture.get();
	}

	bool PostProcessPipeline::Import(rapidxml::xml_node<>* pXmlElem)
	{
		return true;
	}

	bool PostProcessPipeline::Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc)
	{
		rapidxml::xml_node<>* pXmlRTlist = rapidxml::append_node(pXmlElem, doc, "RenderTargetList");
		for (auto it = m_vecTextureInfo.begin(); it != m_vecTextureInfo.end(); ++it)
		{
			rapidxml::xml_node<>* pXmlRT = rapidxml::append_node(pXmlRTlist, doc, "RenderTarget");

			TextureInfo& info = *it;
			rapidxml::append_attribute(pXmlRT, doc, "name", info.m_strName.c_str());
			rapidxml::append_attribute(pXmlRT, doc, "format", info.m_strFormat.c_str());
			if (info.m_fScale > 0.0f)
			{
				rapidxml::append_attribute(pXmlRT, doc, "scale", StringConverter::toString(info.m_fScale).c_str());
			}
			else
			{
				rapidxml::append_attribute(pXmlRT, doc, "width", StringConverter::toString(info.m_nWidth).c_str());
				rapidxml::append_attribute(pXmlRT, doc, "height", StringConverter::toString(info.m_nHight).c_str());
			}
		}

		return true;
	}

	PostProcessPipeline* g_pPostProcessPipeline = NULL;
}
