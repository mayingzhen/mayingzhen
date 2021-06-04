#include "RenderState.h"


namespace ma
{

	RasterizerState::RasterizerState()
	{
		m_fConstantBias = 0;
		m_fSlopeScaleBias = 0;
		m_eCullMode = CULL_FACE_SIDE_BACK;
		m_eFillMode = FILL_SOLID;
		m_bScissorTest = false;
	}

	void RasterizerState::RegisterAttribute()
	{
		ENUM_ATTRIBUTE(RasterizerState, "CullMode", m_eCullMode, CULL_MODE, strCullMode, CULL_FACE_SIDE_BACK, AM_DEFAULT);
	}

	RefPtr<RasterizerState> CreateRasterizerState()
	{
		return GetRenderDevice()->CreateRasterizerState();
	}

	DepthStencilState::DepthStencilState()
	{
	}

	void DepthStencilState::RegisterAttribute()
	{
		ATTRIBUTE(DepthStencilState, "DepthWrite", m_bDepthWrite, bool, true, AM_DEFAULT);
		ENUM_ATTRIBUTE(DepthStencilState, "DepthCheckMode", m_eDepthCheckMode, CompareFunction, strCompareFunction, CMPF_LESS_EQUAL, AM_DEFAULT);
		ATTRIBUTE(DepthStencilState, "Stencil", m_bStencil, bool, false, AM_DEFAULT);
		ENUM_ATTRIBUTE(DepthStencilState, "Stencilfunc", m_eStencilfunc, CompareFunction, strCompareFunction, CMPF_ALWAYS_PASS, AM_DEFAULT);
		ENUM_ATTRIBUTE(DepthStencilState, "StencilPass", m_eStencilPass, StencilOperation, strStencilOperation, SOP_KEEP, AM_DEFAULT);
		ENUM_ATTRIBUTE(DepthStencilState, "StencilFail ", m_eStencilFail, StencilOperation, strStencilOperation, SOP_KEEP, AM_DEFAULT);
		ENUM_ATTRIBUTE(DepthStencilState, "DepthFailOp", m_eDepthFailOp, StencilOperation, strStencilOperation, SOP_KEEP, AM_DEFAULT);
		ATTRIBUTE(DepthStencilState, "StencilMask", m_nStencilMask, uint32_t, -1, AM_DEFAULT);
		ATTRIBUTE(DepthStencilState, "WriteMask", m_nStencilWriteMask, uint32_t, -1, AM_DEFAULT);
	}

	RefPtr<DepthStencilState> CreateDepthStencilState()
	{
		return GetRenderDevice()->CreateDepthStencilState();
	}

	RENDER_TARGET_BLEND_DESC::RENDER_TARGET_BLEND_DESC()
	{

	}


	void RENDER_TARGET_BLEND_DESC::RegisterAttribute()
	{
		ATTRIBUTE(RENDER_TARGET_BLEND_DESC, "BlendEnable", BlendEnable, bool, false, AM_DEFAULT);
		ENUM_ATTRIBUTE(RENDER_TARGET_BLEND_DESC, "SrcBlend", SrcBlend, BLEND, strBLEND, BLEND_SRC_ALPHA, AM_DEFAULT);
		ENUM_ATTRIBUTE(RENDER_TARGET_BLEND_DESC, "DestBlend", DestBlend, BLEND, strBLEND, BLEND_INV_SRC_ALPHA, AM_DEFAULT);
		ENUM_ATTRIBUTE(RENDER_TARGET_BLEND_DESC, "BlendOp", BlendOp, BLEND_OP, strBLEND_OP, BLEND_OP_ADD, AM_DEFAULT);
		ENUM_ATTRIBUTE(RENDER_TARGET_BLEND_DESC, "SrcBlendAlpha", SrcBlendAlpha, BLEND, strBLEND, BLEND_SRC_ALPHA, AM_DEFAULT);
		ENUM_ATTRIBUTE(RENDER_TARGET_BLEND_DESC, "DestBlendAlpha", DestBlendAlpha, BLEND, strBLEND, BLEND_INV_SRC_ALPHA, AM_DEFAULT);
		ENUM_ATTRIBUTE(RENDER_TARGET_BLEND_DESC, "BlendOpAlpha", BlendOpAlpha, BLEND_OP, strBLEND_OP, BLEND_OP_ADD, AM_DEFAULT);
		ATTRIBUTE(RENDER_TARGET_BLEND_DESC, "nColorWrite", nColorWrite, int, 0xF, AM_DEFAULT);
	}

	BlendState::BlendState()
	{
	}

	void BlendState::RegisterAttribute()
	{

	}

	bool BlendState::Import(rapidxml::xml_node<>* pXmlObject)
	{
		rapidxml::xml_node<>* pXmlSubDesc = pXmlObject->first_node("RENDER_TARGET_BLEND_DESC");
		while (pXmlSubDesc)
		{
			const char* pszIndex = pXmlSubDesc->findAttribute("Index");
			ASSERT(pszIndex);
			int Index = StringConverter::parseInt(pszIndex);
			ASSERT(Index >= 0 && Index < MAX_RENDERTARGETS);
			if (Index >= 0 && Index < MAX_RENDERTARGETS)
			{
				m_blendDesc[Index].Import(pXmlSubDesc);
			}

			pXmlSubDesc = pXmlSubDesc->next_sibling("RENDER_TARGET_BLEND_DESC");
		}

		return true;
	}

	bool BlendState::Export(rapidxml::xml_node<>* pXmlObject, rapidxml::xml_document<>& doc)
	{
		RENDER_TARGET_BLEND_DESC desc_default;
		for (uint32_t i = 0; i < MAX_RENDERTARGETS; ++i)
		{
			if (m_blendDesc[i] == desc_default)
			{
				continue;
			}

			rapidxml::xml_node<>* pXmlSubDesc = doc.allocate_node(rapidxml::node_element, doc.allocate_string("RENDER_TARGET_BLEND_DESC"));
			pXmlSubDesc->append_attribute(doc.allocate_attribute(doc.allocate_string("Index"), doc.allocate_string(StringConverter::toString(i).c_str())));

			m_blendDesc[i].Export(pXmlSubDesc, doc);

			pXmlObject->append_node(pXmlSubDesc);
		}

		return true;
	}

	RefPtr<BlendState> CreateBlendState()
	{
		return GetRenderDevice()->CreateBlendState();
	}

}
