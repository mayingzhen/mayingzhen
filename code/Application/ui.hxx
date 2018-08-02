#include "ui.h"
#include "imgui.h"

namespace ma
{
	class UIRenderable : public Renderable
	{
	public:
		virtual void Render(Technique* pTechnique, RenderCommand* pRenderCommand)
		{
			pRenderCommand->SetScissor(0, 1, &m_scissor);

			Renderable::Render(pTechnique, pRenderCommand);
		}

	public:
		Vector4 m_scissor;
	};

	UI::UI()
	{

	}

	UI::~UI()
	{

	}

	void UI::Init()
	{
		void* hWND = GetRenderSystem()->GetMainWnd();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Setup style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		VertexElement element[3];
		element[0] = VertexElement(0, 0, DT_FLOAT2, DU_POSITION, 0);
		element[1] = VertexElement(0, 8, DT_FLOAT2, DU_TEXCOORD, 0);
		element[2] = VertexElement(0, 16, DT_COLOR, DU_COLOR, 0);
		RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element, 3);

		RefPtr<BlendState> pBlendState = CreateBlendState();
		pBlendState->m_blendDesc[0].BlendEnable = true;
		pBlendState->m_blendDesc[0].SrcBlend = BLEND_SRC_ALPHA;
		pBlendState->m_blendDesc[0].DestBlend = BLEND_INV_SRC_ALPHA;
		pBlendState->m_blendDesc[0].SrcBlendAlpha = BLEND_INV_SRC_ALPHA;
		pBlendState->m_blendDesc[0].DestBlendAlpha = BLEND_ZERO;

		RefPtr<RasterizerState> pRastate = CreateRasterizerState();
		pRastate->m_eCullMode = CULL_FACE_SIDE_NONE;

		ShaderCreateInfo info;
		info.m_strVSFile = "ui.hlsl:vs_main";
		info.m_strPSFile = "ui.hlsl:ps_main";
		info.m_pVertexDecl = pDeclaration;
		info.m_pBlendState = pBlendState;
		info.m_pRSState = pRastate;
		m_pTechUI = CreateTechnique("ui", info);

		// CreateFontTexture
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
		size_t upload_size = width*height * 4 * sizeof(char);

		ImageData data;
		data.m_eFormat = PF_A8B8G8R8;
		data.m_nWidth = width;
		data.m_nHeight = height;
		data.m_nSize = upload_size;
		data.m_pMemory = CreateMemoryStream("", pixels, upload_size, true);
		RefPtr<Texture> gFontTex = CreateTexture(data, false, true);
		m_pFontSampler = CreateSamplerState(gFontTex.get());

		// Store our identifier
		io.Fonts->TexID = (void *)(intptr_t)m_pFontSampler.get();
	}

	void UI::Shutdown()
	{
		m_pTechUI = NULL;
		m_pFontSampler = NULL;
	}

	void UI::Update()
	{
		ImGui::NewFrame();
	}

	UIRenderable* UI::GetRenderable(uint32_t nIndex)
	{
		if (nIndex >= m_vecRendeable.size())
		{
			UIRenderable* pRenderable = new UIRenderable();
			pRenderable->m_pSubMeshData = CreateSubMeshData();
			pRenderable->m_pSubMaterial = CreateSubMaterial();
			pRenderable->m_pSubMaterial->SetShadingTechnqiue(m_pTechUI.get());
			m_vecRendeable.push_back(pRenderable);

			return pRenderable;
		}
		else
		{
			return m_vecRendeable[nIndex].get();
		}

	}

	void UI::RenderDrawData(ImDrawData* draw_data)
	{
		if (draw_data->TotalVtxCount <= 0)
			return;

		ParallHardWareBuffer* pUIHB = GetRenderSystem()->GetUIBufeer();
		pUIHB->LockVideoMemory();

		SubAllocVB subvb = pUIHB->AllocVertexBuffer(draw_data->TotalVtxCount);
		SubAllocIB subib = pUIHB->AllocIndexBuffer(draw_data->TotalIdxCount);

		ImDrawVert* vtx_dst = (ImDrawVert*)subvb.m_pVertices;
		ImDrawIdx* idx_dst = (ImDrawIdx*)subib.m_pIndices;
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
			memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
			vtx_dst += cmd_list->VtxBuffer.Size;
			idx_dst += cmd_list->IdxBuffer.Size;
		}
		pUIHB->UnLockVideoMemory();

		{
			Vector4 vScaleTrans;
			vScaleTrans.x = 2.0f / draw_data->DisplaySize.x;
			vScaleTrans.y = 2.0f / draw_data->DisplaySize.y;
			vScaleTrans.z = -1.0f - draw_data->DisplayPos.x * vScaleTrans.x;
			vScaleTrans.w = -1.0f - draw_data->DisplayPos.y * vScaleTrans.y;

			m_pTechUI->SetValue(m_pTechUI->GetUniform(VS,"uScaleTranslate"), vScaleTrans);
		}

		m_pTechUI->SetValue(m_pTechUI->GetUniform(PS,"texture0"), m_pFontSampler.get());

		uint32_t nUsedCount = 0;
		
		// Render command lists
		int vtx_offset = 0;
		int idx_offset = 0;
		ImVec2 display_pos = draw_data->DisplayPos;
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					// User callback (registered via ImDrawList::AddCallback)
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					// Apply scissor/clipping rectangle
					// FIXME: We could clamp width/height based on clamped min/max values.
					Vector4 scissor;
					scissor.x = (int32_t)(pcmd->ClipRect.x - display_pos.x) > 0 ? (int32_t)(pcmd->ClipRect.x - display_pos.x) : 0;
					scissor.y = (int32_t)(pcmd->ClipRect.y - display_pos.y) > 0 ? (int32_t)(pcmd->ClipRect.y - display_pos.y) : 0;
					scissor.z = (uint32_t)(pcmd->ClipRect.z - pcmd->ClipRect.x);
					scissor.w = (uint32_t)(pcmd->ClipRect.w - pcmd->ClipRect.y + 1); // FIXME: Why +1 here?

					UIRenderable* pRenderable = GetRenderable(nUsedCount++);
					pRenderable->m_pVertexBuffer = pUIHB->GetVertexBuffer();
					pRenderable->m_pIndexBuffer = pUIHB->GetIndexBuffer();
					pRenderable->m_pSubMeshData->m_nIndexCount = pcmd->ElemCount;
					pRenderable->m_pSubMeshData->m_nIndexStart = idx_offset;
					pRenderable->m_pSubMeshData->m_nVertexStart = vtx_offset;
					
					pRenderable->m_scissor = scissor;

					RenderQueue* pRendeQueue = GetRenderSystem()->GetScene()->GetRenderQueue();
					pRendeQueue->AddRenderObj(RL_UI, pRenderable);
				}
				idx_offset += pcmd->ElemCount;
			}
			vtx_offset += cmd_list->VtxBuffer.Size;
		}
	}

	void UI::Render()
	{
		ImGui::Render();

		RenderDrawData(ImGui::GetDrawData());
	}

	bool UI::Begin(const char* name, bool* p_open, int flags)
	{
		return ImGui::Begin(name, p_open, flags);
	}

	void UI::End()
	{
		ImGui::End();
	}

	void UI::SameLine(float pos_x/* = 0.0f*/, float spacing_w/* = -1.0f*/)
	{
		ImGui::SameLine(pos_x, spacing_w);
	}

	bool UI::Header(const char *caption)
	{
		return ImGui::CollapsingHeader(caption, ImGuiTreeNodeFlags_DefaultOpen);
	}

	bool UI::Checkbox(const char *caption, bool *value)
	{
		return ImGui::Checkbox(caption, value);
	}

	bool UI::InputFloat(const char *caption, float *value, float step, uint32_t precision)
	{
		return ImGui::InputFloat(caption, value, step, step * 10.0f, precision);
	}

	bool UI::SliderFloat(const char* caption, float* value, float min, float max)
	{
		return ImGui::SliderFloat(caption, value, min, max);
	}

	bool UI::SliderInt(const char* caption, int32_t* value, int32_t min, int32_t max)
	{
		return ImGui::SliderInt(caption, value, min, max);
	}

	bool UI::ComboBox(const char *caption, int32_t *itemindex, std::vector<std::string> items)
	{
		if (items.empty()) {
			return false;
		}
		std::vector<const char*> charitems;
		charitems.reserve(items.size());
		for (size_t i = 0; i < items.size(); i++) {
			charitems.push_back(items[i].c_str());
		}
		uint32_t itemCount = static_cast<uint32_t>(charitems.size());
		return ImGui::Combo(caption, itemindex, &charitems[0], itemCount, itemCount);
	}

	bool UI::Button(const char *caption)
	{
		return ImGui::Button(caption);
	}

	void UI::Text(const char *formatstr, ...)
	{
		va_list args;
		va_start(args, formatstr);
		ImGui::TextV(formatstr, args);
		va_end(args);
	}

	float UI::GetFramerate()
	{
		return ImGui::GetIO().Framerate;
	}

	UI* g_pUI = NULL;
	APP_API UI* GetUI()
	{
		return g_pUI;
	}
}
