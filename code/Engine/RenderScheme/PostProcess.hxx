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
		//m_strTech = pszTech;
		m_pMaterial = CreateSubMaterial();

		RefPtr<Technique> pTech = CreateTechnique(pszTech, "", m_pRenderPass.get());
		m_pMaterial->SetShadingTechnqiue(pTech.get());
	}

	void PostProcessStep::Setup(RenderPass* pInFB, RenderPass* pOutFB)
	{
		m_pRenderPass = g_pPostProcessPipeline->GetRenderPass(m_strTargetName.c_str());
		if (m_strTargetName == "[StageOutput]")
		{
			m_pRenderPass = pOutFB;
		}
		ASSERT(m_pRenderPass);

// 		RefPtr<Technique> pTech = m_pMaterial->GetShadingTechnqiue();
// 		pTech->SetRenderPass(m_pRenderPass.get());
// 		pTech->ReLoad();
		SetTechnique(m_pMaterial->GetShadingTechnqiue()->GetTechName());

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
		RefPtr<Technique> pTech = m_pMaterial->GetShadingTechnqiue();

		ScreenQuad::Render(pTech.get(), m_pRenderPass->GetThreadCommand(0, 0));
	}

	const char* PostProcessStep::GetName()
	{
		return m_strName.c_str();
	}

	void PostProcessStep::SetName(const char* pszName)
	{
		m_strName = pszName;
	}

	void PostProcessStep::Import(rapidxml::xml_node<>* pXmlElem)
	{
		m_strName = pXmlElem->findAttribute("Name");

		m_strTargetName = pXmlElem->findAttribute("TargetName");

		rapidxml::xml_node<>* pXmlInTexture = pXmlElem->first_node("InTexture");
		while (pXmlInTexture)
		{
			const char* pszParamName = pXmlInTexture->findAttribute("ParamName");
			const char* pszTextureName = pXmlInTexture->findAttribute("TextureName");

			SetInput(pszParamName, pszTextureName);

			pXmlInTexture = pXmlInTexture->next_sibling("InTexture");
		}

		rapidxml::xml_node<>* pXmlMaterial = pXmlElem->first_node("Material");
		m_pMaterial = CreateSubMaterial();
		m_pMaterial->Import(pXmlMaterial,NULL);
	}

	void PostProcessStep::Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc)
	{
		rapidxml::append_attribute(pXmlElem, doc, "Name", m_strName.c_str());

		rapidxml::append_attribute(pXmlElem, doc, "TargetName", m_strTargetName.c_str());

		for (auto it = m_strInTexture.begin(); it != m_strInTexture.end(); ++it)
		{
			rapidxml::xml_node<>* pXmlInTexture = rapidxml::append_node(pXmlElem, doc, "InTexture");

			rapidxml::append_attribute(pXmlInTexture, doc, "ParamName", it->first.c_str());
			rapidxml::append_attribute(pXmlInTexture, doc, "TextureName", it->second.c_str());
		}

		rapidxml::xml_node<>* pXmlMaterial = rapidxml::append_node(pXmlElem, doc, "Material");
		m_pMaterial->Export(pXmlMaterial,doc);
	}

	const char* PostProcess::GetName()
	{
		return m_strName.c_str();
	}

	void PostProcess::SetName(const char* pszName)
	{
		m_strName = pszName;
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
			m_vecStep[i]->GetRenderPass()->Begine();

			m_vecStep[i]->Render();

			m_vecStep[i]->GetRenderPass()->End();
		}
	}

	void PostProcess::Import(rapidxml::xml_node<>* pXmlElem)
	{
		m_strName = pXmlElem->findAttribute("Name");

		rapidxml::xml_node<>* pXmlStep = pXmlElem->first_node("Step");
		while (pXmlStep)
		{
			RefPtr<PostProcessStep> pStep = new PostProcessStep();
			pStep->Import(pXmlStep);

			this->AddStep(pStep.get());

			pXmlStep = pXmlStep->next_sibling("Step");
		}
	}

	void PostProcess::Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc)
	{
		rapidxml::append_attribute(pXmlElem, doc, "Name", m_strName.c_str());

		for (uint32_t i = 0; i < m_vecStep.size(); ++i)
		{
			rapidxml::xml_node<>* pXmlStep = rapidxml::append_node(pXmlElem, doc, "Step");
			m_vecStep[i]->Export(pXmlStep, doc);
		}
	}

	PostProcessStep* PostProcess::GetStep(const char* pszName)
	{
		for (uint32_t i = 0; i < m_vecStep.size(); ++i)
		{
			if (strcmp(m_vecStep[i]->GetName(), pszName) == 0)
			{
				return m_vecStep[i].get();
			}
		}

		return nullptr;
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

	PixelFormat ParaseFormat(const std::string strFormat)
	{
		if (strFormat == "PF_FLOAT32_R")
		{
			return PF_FLOAT32_R;
		}
		else if (strFormat == "PF_A8R8G8B8")
		{
			return PF_A8R8G8B8;
		}
		else
		{
			ASSERT(false);
			return PF_A8R8G8B8;
		}
	}

	std::string ToStringFormat(PixelFormat format)
	{
		if (format == PF_FLOAT32_R)
		{
			return "PF_FLOAT32_R";
		}
		else if (format == PF_A8R8G8B8)
		{
			return "PF_A8R8G8B8";
		}
		else
		{
			ASSERT(false);
			return "PF_A8R8G8B8";
		}
	}

	void PostProcessPipeline::AddRenderPass(const char* pszName, float fScale, PixelFormat format, bool bSRGB)
	{
		TextureInfo info;
		info.m_strName = pszName;
		info.m_fScale = fScale;
		info.m_strFormat = ToStringFormat(format);
		info.m_sRGB = bSRGB;
		m_vecTextureInfo.push_back(info);
	}

	void PostProcessPipeline::AddRenderPass(const char* pszName, int nWidth, int nHeight, PixelFormat format, bool bSRGB)
	{
		TextureInfo info;
		info.m_strName = pszName;
		info.m_nWidth = nWidth;
		info.m_nHight = nHeight;
		info.m_strFormat = ToStringFormat(format);
		info.m_sRGB = bSRGB;
		m_vecTextureInfo.push_back(info);
	}

	void PostProcessPipeline::AddPostProcess(PostProcess* pPostProcess)
	{
		m_vecPostProcess.push_back(pPostProcess);
	}

	PostProcess* PostProcessPipeline::GetPostProcess(const char* pszName)
	{
		for (uint32_t i = 0; i < m_vecPostProcess.size(); ++i)
		{
			if (strcmp(m_vecPostProcess[i]->GetName(), pszName) == 0)
			{
				return m_vecPostProcess[i].get();
			}
		}

		return nullptr;
	}

	RenderPass* PostProcessPipeline::GetRenderPass(const char* pszName)
	{
		for (uint32_t i = 0; i < m_vecTextureInfo.size(); ++i)
		{
			TextureInfo& info = m_vecTextureInfo[i];
			if (strcmp(pszName, info.m_strName.c_str()) != 0)
				continue;

			if (info.m_pRenderPass)
				return info.m_pRenderPass.get();

			int nWidth = info.m_nWidth;
			int nHeight = info.m_nHight;
			if (info.m_fScale > 0.0f)
			{
				Rectangle rect = GetRenderSystem()->GetViewPort();
				nWidth = (int)(info.m_fScale * rect.width());
				nHeight = (int)(info.m_fScale * rect.height());
			}

			PixelFormat format = ParaseFormat(info.m_strFormat);

			info.m_pRenderPass = GetRenderDevice()->CreateRenderPass();
			RefPtr<Texture> pTex = GetRenderSystem()->CreateRenderTarget(nWidth, nHeight, 1, format, info.m_sRGB);
			info.m_pRenderPass->AttachColor(0, pTex.get(), 0, 0);
			GetRenderSystem()->RenderPassStreamComplete(info.m_pRenderPass.get());
			
			return info.m_pRenderPass.get();

		}
		
		return nullptr;
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
		rapidxml::xml_node<>* pXmlRTlist = pXmlElem->first_node("RenderTargetList");
		rapidxml::xml_node<>* pXmlRT = pXmlRTlist->first_node("RenderTarget");
		while (pXmlRT)
		{
			const char* pszName = pXmlRT->findAttribute("name");
			const char* pszFormat = pXmlRT->findAttribute("format");
			const char* pszScale = pXmlRT->findAttribute("scale");
			const char* pszWidth = pXmlRT->findAttribute("width");
			const char* pszHeight = pXmlRT->findAttribute("height");
			const char* pszSRGB = pXmlRT->findAttribute("srgb");

			TextureInfo info;
			info.m_strName = pszName;
			info.m_strFormat = pszFormat;
			info.m_sRGB = StringConverter::parseBool(pszSRGB);
			if (pszScale)
			{
				info.m_fScale = StringConverter::parseReal(pszScale);
			}
			else
			{
				info.m_nWidth = StringConverter::parseInt(pszWidth);
				info.m_nHight = StringConverter::parseInt(pszHeight);
			}

			m_vecTextureInfo.push_back(info);

			pXmlRT = pXmlRT->next_sibling("RenderTarget");
		}

		rapidxml::xml_node<>* pXmlPostProcesslist = pXmlElem->first_node("PostProcesslist");
		rapidxml::xml_node<>* pXmlPostProcess = pXmlPostProcesslist->first_node("PostProcess");
		while (pXmlPostProcess)
		{
			RefPtr<PostProcess> pPostProcess = new PostProcess();
			pPostProcess->Import(pXmlPostProcess);
			this->AddPostProcess(pPostProcess.get());

			pXmlPostProcess = pXmlPostProcess->next_sibling("PostProcess");
		}


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
			rapidxml::append_attribute(pXmlRT, doc, "srgb", StringConverter::toString(info.m_sRGB).c_str());
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

		rapidxml::xml_node<>* pXmlPostProcesslist = rapidxml::append_node(pXmlElem, doc, "PostProcesslist");
		for (uint32_t i = 0; i < m_vecPostProcess.size(); ++i)
		{
			rapidxml::xml_node<>* pXmlPostProcess = rapidxml::append_node(pXmlPostProcesslist, doc, "PostProcess");

			m_vecPostProcess[i]->Export(pXmlPostProcess, doc);
		}

		return true;
	}

	PostProcessPipeline* g_pPostProcessPipeline = NULL;
	PostProcessPipeline* GetPostProcess()
	{
		return g_pPostProcessPipeline;
	}
}
