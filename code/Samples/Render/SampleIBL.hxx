#include "Samples/Render/SampleIBL.h"
#include "imgui.h"
#include "imgui_impl_win32.h"

namespace ma
{
	SampleIBL::SampleIBL()
	{
	}

	PrefilterCube gFilterCube;

	RefPtr<Technique> gTechUI;

	RefPtr<SamplerState> gFont;

	bool ImGui_ImplVulkan_CreateFontsTexture()
	{
		ImGuiIO& io = ImGui::GetIO();

		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
		size_t upload_size = width*height * 4 * sizeof(char);

		
		ImageData data;
		data.m_eFormat = PF_A8R8G8B8;
		data.m_nWidth = width;
		data.m_nHeight = height;
		data.m_nSize = upload_size;
		data.m_pMemory = CreateMemoryStream("", pixels, upload_size, true);
		RefPtr<Texture> gFontTex = CreateTexture(data, false, true);
		gFont = CreateSamplerState(gFontTex.get());



		// Store our identifier
		io.Fonts->TexID = (void *)(intptr_t)gFont.get();

		return true;
	}

	void SetUpImGUI()
	{
		void* hWND = GetRenderSystem()->GetMainWnd();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplWin32_Init(hWND);

		// Setup style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		gTechUI = CreateTechnique("shader/ui.tech","",NULL);

		ImGui_ImplVulkan_CreateFontsTexture();
	}

	void SampleIBL::Load()
	{
		SetUpImGUI();

		GetCamera()->LookAt(Vector3(5, 3, -5), Vector3(0, 0, 0));
		GetCameraControll()->Init();

		CreateMeshMaterial("FBX/Box.tga","FBX/Box.mtl", "IBL");

		gFilterCube.Init("env.dds");

		RefPtr<SceneNode> pSkyBoxNode = m_pScene->CreateSceneNode();
		RefPtr<SkyBox> pSkyBox = pSkyBoxNode->CreateComponent<SkyBox>();
		pSkyBox->SetCubeMap("env.dds");

		for (uint32_t iM = 0; iM < 0; ++iM)
		{
			for (uint32_t iR = 0; iR < 0; ++iR)
			{
				RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();

				pShpere->SetScale(Vector3(0.4f));
				pShpere->Translate(Vector3(iM + 0.0f, 0, iR + 0.0f));

				RefPtr<MeshComponent> pMeshComp = pShpere->CreateComponent<MeshComponent>();
				pMeshComp->Load("Fbx/shpere.skn","Fbx/Box.mtl");

				RefPtr<Material> pMaterial = CreateMaterial("Fbx/Box.mtl");
				RefPtr<Material> pMaterialInst = pMaterial->Clone();

				SubMaterial* pSubMaterial = pMaterialInst->GetSubByIndex(0);

				pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("SPEC", true);
				pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("IBL", true);

				RefPtr<SamplerState> pBRDFTerm = CreateSamplerState("brdf.dds", CLAMP, TFO_POINT, false);
				pSubMaterial->SetParameter("tBRDF", Any(pBRDFTerm));

				//RefPtr<SamplerState> pEnv = CreateSamplerState("env_filtered.dds", REPEAT, TFO_TRILINEAR, false);
				RefPtr<SamplerState> pEnv = CreateSamplerState(gFilterCube.GetOutTexture(), REPEAT, TFO_TRILINEAR, false);
				pSubMaterial->SetParameter("tEnv", Any(pEnv));

				int nMip = pEnv->GetTexture()->GetMipMapNumber();
				Vector2 u_diff_spec_mip(nMip, nMip - 1);

				pSubMaterial->SetParameter("u_diff_spec_mip", Any(u_diff_spec_mip));
		
				float u_metalness = (float)iM / 10.0f;
				float u_glossiness = (float)iR / 10.0f;
				pSubMaterial->SetParameter("u_metalness",Any(u_metalness));
				pSubMaterial->SetParameter("u_glossiness",Any(u_glossiness));

				pMeshComp->SetMaterial(pMaterialInst.get());
			}
		}
		

		m_pScene->SetAmbientColor(Vector3(0.0,0.0,0.0));
	}

	void SampleIBL::UnLoad()
	{

	}

	void SampleIBL::Update()
	{
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		static bool show_another_window = false;

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;
			
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
	}

	// Render function
	// (this used to be set in io.RenderDrawListsFn and called by ImGui::Render(), but you can now call this directly from your main loop)
	void ImGui_ImplDX11_RenderDrawData(ImDrawData* draw_data)
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

		// Setup orthographic projection matrix into our constant buffer
		// Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). 
		{
			float L = draw_data->DisplayPos.x;
			float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
			float T = draw_data->DisplayPos.y;
			float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
			float mvp[4][4] =
			{
				{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
				{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
				{ 0.0f,         0.0f,           0.5f,       0.0f },
				{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
			};
			
			Matrix4 matMvp;
			memcpy(&matMvp, mvp, sizeof(float) * 16);
			
			gTechUI->SetValue(gTechUI->GetUniform("ProjectionMatrix"), matMvp);
		}

		gTechUI->SetValue(gTechUI->GetUniform("texture0"), gFont.get());


		RenderPass* pFb = GetRenderSystem()->GetDefaultRenderPass();

		RenderCommand* pRenderCommand = pFb->GetThreadCommand(0, RL_UI);

		pRenderCommand->Begin();

		gTechUI->Bind(NULL);

		//uint32_t nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : this->m_pIndexBuffer->GetNumber();
		//uint32_t nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;

		pRenderCommand->SetTechnique(gTechUI.get());

		VertexBuffer* pVBBuffer = GetRenderSystem()->GetRTInstaneBuffer()->GetVertexBuffer();
		uint32_t nOffset = subvb.m_nFirstVertex * sizeof(ImDrawVert);
		pRenderCommand->SetVertexBuffer(1, pVBBuffer, nOffset);

		IndexBuffer* pIBBuffer = GetRenderSystem()->GetRTInstaneBuffer()->GetIndexBuffer();
		pRenderCommand->SetIndexBuffer(pIBBuffer);

		
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
					//vkCmdSetScissor(command_buffer, 0, 1, &scissor);
					pRenderCommand->SetScissor(0, 1, &scissor);

					// Draw
					//vkCmdDrawIndexed(command_buffer, pcmd->ElemCount, 1, idx_offset, vtx_offset, 0);
					//pRenderCommand->DrawIndex(idx_offset, pcmd->ElemCount, vtx_offset, 1,PRIM_TRIANGLELIST);
				}
				idx_offset += pcmd->ElemCount;
			}
			vtx_offset += cmd_list->VtxBuffer.Size;
		}

		pRenderCommand->End();
	}

	void SampleIBL::PreRender()
	{
		if (1)
		{
			gFilterCube.Render();
			//PrefilterCubeGPU("env.dds", "env_filtered_.dds");

			//GenIntegrateBRDF("BrdfTest_.dds");
		}
	}

	void SampleIBL::PostRender()
	{
		// Rendering
		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void SampleIBL::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;
	}


}



