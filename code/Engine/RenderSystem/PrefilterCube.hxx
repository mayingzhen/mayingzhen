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
		pInSampler->SetWrapModeW(CLAMP);

		RefPtr<Texture> out_tex = GetRenderSystem()->CreateRenderTarget(in_height,in_height,out_num_mipmaps,PF_FLOAT16_RGBA,false,TEXTYPE_CUBE);

		string strMarc = in_tex->GetType() == TEXTYPE_CUBE ? "":"PARABOLOID";
		RefPtr<Technique> PrefilterCubeDiffuse = CreateTechnique("PrefilterCubeDiffuse","PrefilterCubeDiffuse","PrefilterCubeDiffuse",strMarc.c_str());
		RefPtr<Technique> PrefilterCubeSpecular = CreateTechnique("PrefilterCubeSpecular","PrefilterCubeSpecular","PrefilterCubeSpecular",strMarc.c_str());
		
		PrefilterCubeDiffuse->SetParameter("skybox_cube_tex",Any(pInSampler));
		PrefilterCubeSpecular->SetParameter("skybox_cube_tex",Any(pInSampler));

		for (int face = 0; face < 6; ++ face)
		{
			// level 0 保存原始图
			in_tex->CopyTo(out_tex.get(), face, 0);

			for (uint32_t level = 1; level < out_num_mipmaps - 1; ++ level)
			{
				float roughness = static_cast<float>(out_num_mipmaps - 2 - level) / (out_num_mipmaps - 2);
				roughness = pow(8192.0f, roughness);

				Rectangle viewPort(0,0,(float)(in_height >> level),(float)(in_height >> level));
				GetRenderSystem()->SetViewPort(viewPort);

				GetRenderSystem()->SetRenderTarget(0, out_tex.get(), level, 0, face);
				PrefilterCubeSpecular->SetParameter("face",Any(face));
				PrefilterCubeSpecular->SetParameter("roughness",Any(roughness));
				ScreenQuad::Render(PrefilterCubeSpecular.get());
			}

			// 最后一级 保存Diffuse
			{
				Rectangle viewPort(0,0,(float)(in_height >> (out_num_mipmaps - 1)),(float)(in_height >> (out_num_mipmaps - 1)));
				GetRenderSystem()->SetViewPort(viewPort);

				GetRenderSystem()->SetRenderTarget(0, out_tex.get(), out_num_mipmaps - 1, 0, face);
				PrefilterCubeDiffuse->SetParameter("face",Any(face));

				ScreenQuad::Render(PrefilterCubeDiffuse.get());
			}
		}

		string strSaveDir = GetArchiveMananger()->GetSaveDir();
		string strSaveFile = strSaveDir + out_file;
		out_tex->SaveToFile(strSaveFile.c_str());
	}
}
