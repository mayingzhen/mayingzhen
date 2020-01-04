namespace ma
{
	void PrefilterCube::Init(const char* in_file)
	{
		m_pInTexture = CreateTexture(in_file, false, false);
		uint32_t in_width = m_pInTexture->GetWidth();
		uint32_t in_height = m_pInTexture->GetHeight();

		m_nOutMipmaps = 1;
		{
			uint32_t w = in_width;
			while (w > 8)
			{
				++m_nOutMipmaps;

				w = std::max<uint32_t>(1U, w / 2);
			}
		}
		RefPtr<SamplerState> pInSampler = CreateSamplerState(m_pInTexture.get(), CLAMP, TFO_TRILINEAR, false);

		m_pOutTexture = GetRenderSystem()->CreateRenderTarget(in_height, in_height, m_nOutMipmaps, PF_FLOAT16_RGBA, false, TEXTYPE_CUBE);

		VertexElement element[2];
		element[0] = VertexElement(0, 0, DT_FLOAT2, DU_POSITION, 0);
		element[1] = VertexElement(0, 8, DT_FLOAT2, DU_TEXCOORD, 0);
		RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element, 2);

		Matrix3 matRotate3x3 = Matrix3::IDENTITY;
		//matRotate3x3.FromAxisAngle(Vector3::UNIT_X,Radian(-Math::HALF_PI));
		Matrix4 matRotate(matRotate3x3);

		for (int face = 0; face < 6; ++face)
		{
			// level 0 保存原始图
			{
				RefPtr<RenderPass> pRenderPass = GetRenderDevice()->CreateRenderPass();
				pRenderPass->AttachColor(0, m_pOutTexture.get(), 0, face);
				GetRenderSystem()->RenderPassStreamComplete(pRenderPass.get());

				ShaderCreateInfo info;
				info.m_strVSFile = "screen.vert:main";
				info.m_strPSFile = "PrefilterCube.hlsl:CopySrcPS";
				info.m_pVertexDecl = pDeclaration;
				info.m_pRenderPass = pRenderPass.get();
				m_pTechnique[face][0] = CreateTechnique(info);
				m_pRendePass[face][0] = pRenderPass;

				m_pTechnique[face][0]->SetParameter("tSkyBoxCube", Any(pInSampler));
				m_pTechnique[face][0]->SetParameter("face", Any(face));
				m_pTechnique[face][0]->SetParameter("matRotate", Any(matRotate));
			}

			for (uint32_t level = 1; level < m_nOutMipmaps - 1; ++level)
			{
				float roughness = static_cast<float>(level) / (m_nOutMipmaps - 1);

				RefPtr<RenderPass> pRenderPass = GetRenderDevice()->CreateRenderPass();
				pRenderPass->AttachColor(0,  m_pOutTexture.get(), level, face);
				GetRenderSystem()->RenderPassStreamComplete(pRenderPass.get());

				ShaderCreateInfo info;
				info.m_strVSFile = "screen.vert:main";
				info.m_strPSFile = "PrefilterCube.hlsl:PrefilterCubeSpecularPS";
				info.m_pVertexDecl = pDeclaration;
				info.m_pRenderPass = pRenderPass.get();
				m_pTechnique[face][level] = CreateTechnique(info);
				m_pRendePass[face][level] = pRenderPass;

				m_pTechnique[face][level]->SetParameter("tSkyBoxCube", Any(pInSampler));
				m_pTechnique[face][level]->SetParameter("face", Any(face));
				m_pTechnique[face][level]->SetParameter("roughness", Any(roughness));
				m_pTechnique[face][level]->SetParameter("matRotate", Any(matRotate));
			}

			// 最后一级 保存Diffuse
			{
				RefPtr<RenderPass> pRenderPass = GetRenderDevice()->CreateRenderPass();
				pRenderPass->AttachColor(0, m_pOutTexture.get(), m_nOutMipmaps - 1, face);
				GetRenderSystem()->RenderPassStreamComplete(pRenderPass.get());

				ShaderCreateInfo info;
				info.m_strVSFile = "screen.vert:main";
				info.m_strPSFile = "PrefilterCube.hlsl:PrefilterCubeDiffusePS";
				info.m_pVertexDecl = pDeclaration;
				info.m_pRenderPass = pRenderPass.get();
				m_pTechnique[face][m_nOutMipmaps - 1] = CreateTechnique(info);
				m_pRendePass[face][m_nOutMipmaps - 1] = pRenderPass;

				m_pTechnique[face][m_nOutMipmaps - 1]->SetParameter("tSkyBoxCube", Any(pInSampler));
				m_pTechnique[face][m_nOutMipmaps - 1]->SetParameter("face", Any(face));
				m_pTechnique[face][m_nOutMipmaps - 1]->SetParameter("matRotate", Any(matRotate));
			}
		}

	}

	void PrefilterCube::Render()
	{
		for (int face = 0; face < 6; ++face)
		{
			// level 0 保存原始图
			{
				RefPtr<RenderPass> pRenderPass = m_pRendePass[face][0];
				RefPtr<Technique> PrefilterCopySrc = m_pTechnique[face][0];

				RenderCommand* pCommand = pRenderPass->GetThreadCommand(0, 0);

				pRenderPass->Begine();

				pCommand->Begin();

				ScreenQuad::Render(PrefilterCopySrc.get(), pCommand);

				pCommand->End();

				pRenderPass->End();
			}

			for (uint32_t level = 1; level < m_nOutMipmaps - 1; ++level)
			{
				RefPtr<RenderPass> pRenderPass = m_pRendePass[face][level];
				RefPtr<Technique> PrefilterCubeSpecular = m_pTechnique[face][level];

				RenderCommand* pCommand = pRenderPass->GetThreadCommand(0, 0);

				pRenderPass->Begine();

				pCommand->Begin();

				ScreenQuad::Render(PrefilterCubeSpecular.get(), pCommand);

				pCommand->End();

				pRenderPass->End();
			}

			// 最后一级 保存Diffuse
			{
				RefPtr<RenderPass> pRenderPass = m_pRendePass[face][m_nOutMipmaps - 1];
				RefPtr<Technique> PrefilterCubeDiffuse = m_pTechnique[face][m_nOutMipmaps - 1];

				RenderCommand* pCommand = pRenderPass->GetThreadCommand(0, 0);

				pRenderPass->Begine();

				ScreenQuad::Render(PrefilterCubeDiffuse.get(), pCommand);

				pRenderPass->End();
			}
		}
	}

	void PrefilterCube::Save(const char* out_file)
	{
		string strSaveDir = GetArchiveMananger()->GetSaveDir();
		string strSaveFile = strSaveDir + out_file;
		m_pOutTexture->SaveToFile(strSaveFile.c_str());
	}


	void PrefilterCube::GenIntegrateBRDF(const char* out_file)
	{
		//GetRenderSystem()->BegineRender();

		uint32_t const WIDTH = 256;
		uint32_t const HEIGHT = 256;

		RefPtr<Texture> pOutTex = GetRenderSystem()->CreateRenderTarget(WIDTH,HEIGHT,1,PF_FLOAT32_RGBA,false,TEXTYPE_2D);

		VertexElement element[2];
		element[0] = VertexElement(0, 0, DT_FLOAT2, DU_POSITION, 0);
		element[1] = VertexElement(0, 8, DT_FLOAT2, DU_TEXCOORD, 0);
		RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element, 2);

		RefPtr<RenderPass> pRenderPass = GetRenderDevice()->CreateRenderPass();
		pRenderPass->AttachColor(0, pOutTex.get(), 0, 0);
		pRenderPass->m_arrColor[0].m_cClearColor = ColourValue::White;
		GetRenderSystem()->RenderPassStreamComplete(pRenderPass.get());

		ShaderCreateInfo info;
		info.m_strVSFile = "PrefilterBrdf";
		info.m_strPSFile = "PrefilterBrdf";
		info.m_pVertexDecl = pDeclaration;
		info.m_pRenderPass = pRenderPass.get();
		RefPtr<Technique> pPrefilterBrdf = CreateTechnique(info);

		RenderCommand* pCommand = pRenderPass->GetThreadCommand(0, 0);

		pRenderPass->Begine();

		pCommand->Begin();

		ScreenQuad::Render(pPrefilterBrdf.get(), pCommand);

		pCommand->End();

		pRenderPass->End();

		string strSaveDir = GetArchiveMananger()->GetSaveDir();
		string strSaveFile = strSaveDir + out_file;
		pOutTex->SaveToFile(strSaveFile.c_str());

		//GetRenderSystem()->EndRender();
	}
}

