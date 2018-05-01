namespace ma
{
	void PrefilterCubeGPU(const char* in_file, const char* out_file)
	{
		RefPtr<Texture> in_tex = CreateTexture(in_file, false, false);
		uint32_t in_width = in_tex->GetWidth();
		uint32_t in_height = in_tex->GetHeight();

		uint32_t out_num_mipmaps = 1;
		{
			uint32_t w = in_width;
			while (w > 8)
			{
				++ out_num_mipmaps;

				w = std::max<uint32_t>(1U, w / 2);
			}
		}
		RefPtr<SamplerState> pInSampler = CreateSamplerState(in_tex.get(),CLAMP,TFO_TRILINEAR,false);

		RefPtr<Texture> out_tex = GetRenderSystem()->CreateRenderTarget(in_height,in_height,out_num_mipmaps,PF_FLOAT16_RGBA,false,TEXTYPE_CUBE);

		VertexElement element[2];
		element[0] = VertexElement(0, 0, DT_FLOAT2, DU_POSITION, 0);
		element[1] = VertexElement(0, 8, DT_FLOAT2, DU_TEXCOORD, 0);
		RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element, 2);

// 		ShaderCreateInfo copySrc;
// 		copySrc.m_strVSFile = "PrefilterCopySrc";
// 		copySrc.m_strPSFile = "PrefilterCopySrc";
// 		copySrc.m_pVertexDecl = pDeclaration;
// 		RefPtr<Technique> PrefilterCopySrc = CreateTechnique("PrefilterCopySrc", copySrc);
// 
// 		RefPtr<Technique> PrefilterCubeDiffuse = CreateTechnique("PrefilterCubeDiffuse","PrefilterCubeDiffuse","PrefilterCubeDiffuse","", pDeclaration.get());
// 		
// 		RefPtr<Technique> PrefilterCubeSpecular = CreateTechnique("PrefilterCubeSpecular","PrefilterCubeSpecular","PrefilterCubeSpecular","",pDeclaration.get());
		

		for (int face = 0; face < 6; ++ face)
		{
			// level 0 保存原始图
			{
				RefPtr<RenderPass> pRenderPass = GetRenderDevice()->CreateRenderPass();
				pRenderPass->AttachColor(0, out_tex.get(), 0, face);
				GetRenderSystem()->RenderPassStreamComplete(pRenderPass.get());

				ShaderCreateInfo info;
				info.m_strVSFile = "PrefilterCopySrc";
				info.m_strPSFile = "PrefilterCopySrc";
				info.m_pVertexDecl = pDeclaration;
				info.m_pRenderPass = pRenderPass.get();
				RefPtr<Technique> PrefilterCopySrc = CreateTechnique("PrefilterCopySrc", info);

				RenderCommand* pCommand = pRenderPass->GetThreadCommand(0, (RenderListType)0);
				
				pRenderPass->Begine();

				pCommand->Begin();

				PrefilterCopySrc->SetParameter("skybox_cube_tex", Any(pInSampler));
				PrefilterCopySrc->SetParameter("face",Any(face));

				ScreenQuad::Render(PrefilterCopySrc.get(),pCommand);

				pCommand->End();

				pRenderPass->End();
			}

			for (uint32_t level = 1; level < out_num_mipmaps - 1; ++ level)
			{
				float roughness = static_cast<float>(level) / (out_num_mipmaps - 1);

				RefPtr<RenderPass> pRenderPass = GetRenderDevice()->CreateRenderPass();
				pRenderPass->AttachColor(0, out_tex.get(), level, face);
				GetRenderSystem()->RenderPassStreamComplete(pRenderPass.get());

				ShaderCreateInfo info;
				info.m_strVSFile = "PrefilterCubeSpecular";
				info.m_strPSFile = "PrefilterCubeSpecular";
				info.m_pVertexDecl = pDeclaration;
				info.m_pRenderPass = pRenderPass.get();
				RefPtr<Technique> PrefilterCubeSpecular = CreateTechnique("PrefilterCubeSpecular", info);

				RenderCommand* pCommand = pRenderPass->GetThreadCommand(0, (RenderListType)0);

				pRenderPass->Begine();

				pCommand->Begin();

				PrefilterCubeSpecular->SetParameter("skybox_cube_tex", Any(pInSampler));
				PrefilterCubeSpecular->SetParameter("face",Any(face));
				PrefilterCubeSpecular->SetParameter("roughness",Any(roughness));

				ScreenQuad::Render(PrefilterCubeSpecular.get(), pCommand);

				pCommand->End();

				pRenderPass->End();
			}

			// 最后一级 保存Diffuse
			{
				RefPtr<RenderPass> pRenderPass = GetRenderDevice()->CreateRenderPass();
				pRenderPass->AttachColor(0, out_tex.get(), out_num_mipmaps - 1, face);
				GetRenderSystem()->RenderPassStreamComplete(pRenderPass.get());

				ShaderCreateInfo info;
				info.m_strVSFile = "PrefilterCubeDiffuse";
				info.m_strPSFile = "PrefilterCubeDiffuse";
				info.m_pVertexDecl = pDeclaration;
				info.m_pRenderPass = pRenderPass.get();
				RefPtr<Technique> PrefilterCubeDiffuse = CreateTechnique("PrefilterCubeDiffuse", info);

				RenderCommand* pCommand = pRenderPass->GetThreadCommand(0, (RenderListType)0);

				pRenderPass->Begine();

				pCommand->Begin();

				PrefilterCubeDiffuse->SetParameter("skybox_cube_tex", Any(pInSampler));
				PrefilterCubeDiffuse->SetParameter("face",Any(face));

				ScreenQuad::Render(PrefilterCubeDiffuse.get(), pCommand);

				pCommand->End();

				pRenderPass->End();
			}
		}

		string strSaveDir = GetArchiveMananger()->GetSaveDir();
		string strSaveFile = strSaveDir + out_file;
		out_tex->SaveToFile(strSaveFile.c_str());
	}


	void GenIntegrateBRDF(const char* out_file)
	{
		uint32_t const WIDTH = 256;
		uint32_t const HEIGHT = 256;

		RefPtr<Texture> pOutTex = GetRenderSystem()->CreateRenderTarget(WIDTH,HEIGHT,1,PF_FLOAT32_RGBA,false,TEXTYPE_2D);

		VertexElement element[2];
		element[0] = VertexElement(0, 0, DT_FLOAT2, DU_POSITION, 0);
		element[1] = VertexElement(0, 8, DT_FLOAT2, DU_TEXCOORD, 0);
		RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element, 2);

		RefPtr<RenderPass> pRenderPass = GetRenderDevice()->CreateRenderPass();
		pRenderPass->AttachColor(0, pOutTex.get(), 0, 0);
		GetRenderSystem()->RenderPassStreamComplete(pRenderPass.get());

		ShaderCreateInfo info;
		info.m_strVSFile = "PrefilterBrdf";
		info.m_strPSFile = "PrefilterBrdf";
		info.m_pVertexDecl = pDeclaration;
		info.m_pRenderPass = pRenderPass.get();
		RefPtr<Technique> pPrefilterBrdf = CreateTechnique("PrefilterBrdf", info);

		RenderCommand* pCommand = pRenderPass->GetThreadCommand(0, (RenderListType)0);

		pRenderPass->Begine();

		pCommand->Begin();

		ScreenQuad::Render(pPrefilterBrdf.get(), pCommand);

		pCommand->End();

		pRenderPass->End();

		string strSaveDir = GetArchiveMananger()->GetSaveDir();
		string strSaveFile = strSaveDir + out_file;
		pOutTex->SaveToFile(strSaveFile.c_str());
	}
}

