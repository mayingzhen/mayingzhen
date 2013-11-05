#include "BlurPostProcess.h"

namespace ma
{
	DownScalePostProcess::DownScalePostProcess(Texture* pInputTex, Texture* pOutputTex,int nScale/* = 2*/)
		:PostProcess(pInputTex,pOutputTex)
	{
		m_nScale = nScale;
	}

	DownScalePostProcess::~DownScalePostProcess()
	{

	}

	void DownScalePostProcess::Init()
	{
		char pszDefine[MAX_PATH] = {0};
		sprintf(pszDefine,MAX_PATH,"SCALE %d",m_nScale);

		m_DownScaleTech = new Technique("DownSacle","Screen.vert","DownScale/DownScale",pszDefine); 
	}

	void DownScalePostProcess::Shutdown()
	{
		SAFE_DELETE(m_DownScaleTech);
	}

	void DownScalePostProcess::Render()
	{
		RENDER_PROFILE(DownScalePostProcess);

		/// x
		GetRenderSystem()->PushRenderTarget(m_pOutputTex);

		GetRenderSystem()->ClearBuffer(true,true,true,Color(0,0,0,0),1.0f,0);

		float width = m_pInputTex->getWidth();
		float heigh = m_pInputTex->getWidth();


		m_BlurXTech->GetParameter("color_weight")->setFloatArray( &(color_weight[0]), 8 );
		m_BlurXTech->GetParameter("tex_coord_offset")->setFloatArray( &(tex_coord_offset[0]), 8 );
		m_BlurXTech->GetParameter("src_tex_size")->setFloatArray( texSize, 2 );

		m_BlurXTech->GetParameter("g_SamplerSrc")->setTexture(m_pInputTex);

		ScreenQuad::Render(m_BlurXTech);

		GetRenderSystem()->PopRenderTargert();
	}


	void DownScalePostProcess::GetSampleOffsets4x4(int width, int height, Vector4 texCoordOffSet[],int nArray)
	{
		float const tu = 1.0f / width;
		float const tv = 1.0f / height;

		// Sample from the 16 surrounding points.
		int index = 0;
		for (int y = -1; y <= 2; y += 2)
		{
			for (int x = -1; x <= 2; x += 4) 
			{
				if (index >= nArray)
					return;

				texCoordOffSet[index].x = (x + 0) * tu;
				texCoordOffSet[index].y = y * tv;
				texCoordOffSet[index].z = (x + 2) * tu;
				texCoordOffSet[index].w = y * tv;

				++ index;
			}
		}
	}



}