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

		RefPtr<Technique> PrefilterCopySrc = CreateTechnique("PrefilterCopySrc","PrefilterCopySrc","PrefilterCopySrc","", pDeclaration.get());
		RefPtr<Technique> PrefilterCubeDiffuse = CreateTechnique("PrefilterCubeDiffuse","PrefilterCubeDiffuse","PrefilterCubeDiffuse","", pDeclaration.get());
		RefPtr<Technique> PrefilterCubeSpecular = CreateTechnique("PrefilterCubeSpecular","PrefilterCubeSpecular","PrefilterCubeSpecular","",pDeclaration.get());

		PrefilterCopySrc->SetParameter("skybox_cube_tex",Any(pInSampler));
		PrefilterCubeDiffuse->SetParameter("skybox_cube_tex",Any(pInSampler));
		PrefilterCubeSpecular->SetParameter("skybox_cube_tex",Any(pInSampler));

		for (int face = 0; face < 6; ++ face)
		{
			// level 0 保存原始图
			{
				RefPtr<RenderPass> pRenderPass = GetRenderDevice()->CreateRenderPass();
				pRenderPass->AttachColor(0, out_tex.get(), 0, face);
				GetRenderSystem()->RenderPassStreamComplete(pRenderPass.get());

				PrefilterCopySrc->SetRenderPass(pRenderPass.get());
				GetRenderSystem()->TechniqueStreamComplete(PrefilterCopySrc.get());

				RenderCommand* pCommand = pRenderPass->GetThreadCommand(0, (RenderListType)0);
				
				pRenderPass->Begine();

				pCommand->Begin();

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

				PrefilterCubeSpecular->SetRenderPass(pRenderPass.get());
				GetRenderSystem()->TechniqueStreamComplete(PrefilterCubeSpecular.get());

				RenderCommand* pCommand = pRenderPass->GetThreadCommand(0, (RenderListType)0);

				pRenderPass->Begine();

				pCommand->Begin();

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

				PrefilterCubeDiffuse->SetRenderPass(pRenderPass.get());
				GetRenderSystem()->TechniqueStreamComplete(PrefilterCubeDiffuse.get());

				RenderCommand* pCommand = pRenderPass->GetThreadCommand(0, (RenderListType)0);

				pRenderPass->Begine();

				pCommand->Begin();

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

		RefPtr<Technique> pPrefilterBrdf = CreateTechnique("PrefilterBrdf","PrefilterBrdf","PrefilterBrdf","", pDeclaration.get());

		RefPtr<RenderPass> pRenderPass = GetRenderDevice()->CreateRenderPass();
		pRenderPass->AttachColor(0, pOutTex.get(), 0, 0);
		GetRenderSystem()->RenderPassStreamComplete(pRenderPass.get());

		pPrefilterBrdf->SetRenderPass(pRenderPass.get());
		GetRenderSystem()->TechniqueStreamComplete(pPrefilterBrdf.get());

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

