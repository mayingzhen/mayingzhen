#include "public.h"
#include "PostProcess.h"
#include "GameApp.h"

CPostProcess::CPostProcess()
{
}

CPostProcess::~CPostProcess()
{
	for (int i = 0; i < m_pInputTexs.size(); ++i)
	{
		SAFE_RELEASE(m_pInputTexs[i]);
	}

	for (int i = 0; i < m_pOutputTexs.size(); ++i)
	{
		SAFE_RELEASE(m_pOutputTexs[i]);
	}
	
}

void CPostProcess::Init( LPDIRECT3DTEXTURE9 pInputTex,LPDIRECT3DTEXTURE9 pOutputTex,
						const char*  sTechName )
{
	m_Tech.loadResource(sTechName);

	assert( m_pInputTexs.empty() && m_pOutputTexs.empty() );
	m_pInputTexs.push_back(pInputTex);
	m_pOutputTexs.push_back(pOutputTex);

	m_ScreenQuad.Create();
}

void CPostProcess::Init( const std::vector<LPDIRECT3DTEXTURE9>& pInputTexs,
				  LPDIRECT3DTEXTURE9 pOutputTex,
				  const char* sTechName)
{
	m_Tech.loadResource(sTechName);

	assert( m_pInputTexs.empty() && m_pOutputTexs.empty() );
	m_pInputTexs = std::vector<LPDIRECT3DTEXTURE9>( pInputTexs.begin(), pInputTexs.end() );
	m_pOutputTexs.push_back(pOutputTex);
}

void CPostProcess::Render()
{
	m_Tech.begin();
	m_Tech.BeginPass(0);

	OnRenderBegin();

	m_ScreenQuad.Render();

	OnRenderEnd();

	m_Tech.endPass();
	m_Tech.end();
}

void CPostProcess::OnRenderBegin()
{
}

void CPostProcess::OnRenderEnd()
{
}

void CPostProcess::InputTex(int index, LPDIRECT3DTEXTURE9 pInputTex)
{
	if (index < 0)
		return;

	if (m_pInputTexs.size() + 1 < index)
	{
		m_pInputTexs.resize(index + 1);
	}
	m_pInputTexs[index] = pInputTex;
}

void CPostProcess::OutputTex(int index, LPDIRECT3DTEXTURE9 pOutputTex)
{
	if (index < 0)
		return;

	if (m_pOutputTexs.size() + 1 < index)
	{
		m_pOutputTexs.resize(index + 1);
	}
	m_pOutputTexs[index] = pOutputTex;
}


void CPostProcessChain::Init( LPDIRECT3DTEXTURE9 pInputTex,
							  LPDIRECT3DTEXTURE9 pOutputTex,
							  const char* sTechName )
{
	if ( m_ppChain.empty() )
	{
		m_ppChain.resize(1);
	}

	m_ppChain[0].Init( pInputTex, pOutputTex, sTechName);

	if (m_ppChain.size() >1)
	{

	}
}

void CPostProcessChain::Render()
{
	for (int i = 0; i < m_ppChain.size(); ++i)
	{
		m_ppChain.rend();
	}
}


float CSeparableGaussianFilterPostProcess::GaussianDistribution(float x, float y, float rho)
{
	float g = 1.0f / sqrtf( 2.0f * D3DX_PI * rho * rho );
	g *= expf( -( x * x + y * y ) / ( 2 * rho * rho ) );

	return g;
}

void CSeparableGaussianFilterPostProcess::CalSampleOffsets(int texSize,float deviation)
{
	std::vector<float> color_weight(m_KernelRaidius, 0);
	std::vector<float> tex_coord_offset(m_KernelRaidius, 0);

	std::vector<float> tmp_weights(m_KernelRaidius * 2, 0);
	std::vector<float> tmp_offset(m_KernelRaidius * 2, 0);

	float const tu = 1.0f / texSize;

	float sum_weight = 0;
	for (int i = 0; i < 2 * m_KernelRaidius; ++ i)
	{
		float weight = this->GaussianDistribution(static_cast<float>(i - m_KernelRaidius), 0, m_KernelRaidius / deviation);
		tmp_weights[i] = weight;
		sum_weight += weight;
	}
	for (int i = 0; i < 2 * m_KernelRaidius; ++ i)
	{
		tmp_weights[i] /= sum_weight;
	}

	// Fill the offsets
	for (int i = 0; i < m_KernelRaidius; ++ i)
	{
		tmp_offset[i]                  = static_cast<float>(i - m_KernelRaidius);
		tmp_offset[i + m_KernelRaidius] = static_cast<float>(i);
	}

	// Bilinear filtering taps
	// Ordering is left to right.
	for (int i = 0; i < m_KernelRaidius; ++ i)
	{
		float const scale = tmp_weights[i * 2] + tmp_weights[i * 2 + 1];
		float const frac = tmp_weights[i * 2] / scale;

		tex_coord_offset[i] = (tmp_offset[i * 2] + (1 - frac)) * tu;
		color_weight[i] = m_multiplier * scale;
	}

// 	*src_tex_size_ep_ = float2(static_cast<float>(tex_size), 1.0f / tex_size);
// 	*color_weight_ep_ = color_weight;
// 	*tex_coord_offset_ep_ = tex_coord_offset;
}

void CSeparableGaussianFilterPostProcess::Init( LPDIRECT3DTEXTURE9 pInputTex,
											   LPDIRECT3DTEXTURE9 pOutputTex,
											   const char* sTechName )
{
	m_BlureX.Init(pInputTex,pOutputTex,sTechName);
	m_BlureX.
}

void CSeparableGaussianFilterPostProcess::Render()
{

}