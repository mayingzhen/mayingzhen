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

		RefPtr<Technique> PrefilterCubeDiffuse = CreateTechnique("PrefilterCubeDiffuse","PrefilterCubeDiffuse","PrefilterCubeDiffuse","");
		RefPtr<Technique> PrefilterCubeSpecular = CreateTechnique("PrefilterCubeSpecular","PrefilterCubeSpecular","PrefilterCubeSpecular","");
	
		PrefilterCubeDiffuse->SetParameter("skybox_cube_tex",Any(pInSampler));
		PrefilterCubeSpecular->SetParameter("skybox_cube_tex",Any(pInSampler));

		enum eCubeFace
		{
			POSITIVE_X    = 0,
			NEGATIVE_X    = 1,
			POSITIVE_Y    = 2,
			NEGATIVE_Y    = 3,
			POSITIVE_Z    = 4,
			NEGATIVE_Z    = 5,	
		};

		for (int face = 0; face < 6; ++ face)
		{
			int nOutFace = face;
// 			if (face == POSITIVE_Y)
// 			{
// 				nOutFace = POSITIVE_Z;
// 			}
// 			else if (face == NEGATIVE_Y)
// 			{
// 				nOutFace = NEGATIVE_Z;
// 			}
// 			else if (face == POSITIVE_Z)
// 			{
// 				nOutFace = NEGATIVE_Y;
// 			}
// 			else if (face == NEGATIVE_Z)
// 			{
// 				nOutFace = POSITIVE_Y;
// 			}

			// level 0 保存原始图
			//in_tex->CopyTo(out_tex.get(), nOutFace,0, face, 0);

			for (uint32_t level = 0; level < out_num_mipmaps - 1; ++ level)
			{
				float roughness = static_cast<float>(out_num_mipmaps - 1 - level) / (out_num_mipmaps - 1);
				roughness  = 1.0f - roughness;
				//roughness = pow(8192.0f, roughness);

				Rectangle viewPort(0,0,(float)(in_height >> level),(float)(in_height >> level));
				GetRenderSystem()->SetViewPort(viewPort);

				GetRenderSystem()->SetRenderTarget(0, out_tex.get(), level, 0, nOutFace);

				PrefilterCubeSpecular->SetParameter("face",Any(face));
				PrefilterCubeSpecular->SetParameter("roughness",Any(roughness));

				ScreenQuad::Render(PrefilterCubeSpecular.get());
			}

			// 最后一级 保存Diffuse
			{
				Rectangle viewPort(0,0,(float)(in_height >> (out_num_mipmaps - 1)),(float)(in_height >> (out_num_mipmaps - 1)));
				GetRenderSystem()->SetViewPort(viewPort);

				GetRenderSystem()->SetRenderTarget(0, out_tex.get(), out_num_mipmaps - 1, 0, nOutFace);

				PrefilterCubeDiffuse->SetParameter("face",Any(face));

				ScreenQuad::Render(PrefilterCubeDiffuse.get());
			}
		}

		string strSaveDir = GetArchiveMananger()->GetSaveDir();
		string strSaveFile = strSaveDir + out_file;
		out_tex->SaveToFile(strSaveFile.c_str());
	}
}
