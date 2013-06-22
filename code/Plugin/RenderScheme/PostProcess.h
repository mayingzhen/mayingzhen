#ifndef  C_POST_PROCESS__H__
#define  C_POST_PROCESS__H__

#include <vector>

#include "ScreenQuad.h"

class CPostProcess
{
public:
	CPostProcess();
	~CPostProcess();

	virtual void Init( LPDIRECT3DTEXTURE9 pInputTex,
					   LPDIRECT3DTEXTURE9 pOutputTex,
					   const char* sTechName );
	virtual void Init( const std::vector<LPDIRECT3DTEXTURE9>& pInputTexs,
					   LPDIRECT3DTEXTURE9 pOutputTex,
					   const char* sTechName);

	void InputTex(int index, LPDIRECT3DTEXTURE9 pInputTex);
	void OutputTex(int index, LPDIRECT3DTEXTURE9 pOutputTex);

	virtual void Render();
	virtual void OnRenderBegin();
	virtual void OnRenderEnd();

private:
	std::vector<LPDIRECT3DTEXTURE9> m_pInputTexs;
	std::vector<LPDIRECT3DTEXTURE9> m_pOutputTexs;

	//CEffectFile	m_Tech;
	Material*		m_pMaterial;

	ScreenQuad		m_ScreenQuad;
};

class CPostProcessChain 
{
	void Init( LPDIRECT3DTEXTURE9 pInputTex,
		LPDIRECT3DTEXTURE9 pOutputTex,
		const char* sTechName );

	void Render();

	void Append(const CPostProcess& pp) {m_ppChain.push_back(pp)};

private:
	std::vector<CPostProcess> m_ppChain;

	std::vector<LPDIRECT3DTEXTURE9> m_tempTexture;

};


class CSeparableGaussianFilterPostProcess : public CPostProcessChain
{
public:
	void Init( LPDIRECT3DTEXTURE9 pInputTex,
		LPDIRECT3DTEXTURE9 pOutputTex,
		const char* sTechName );
	void Render();	

private:
	float GaussianDistribution(float x, float y, float rho);
	void CalSampleOffsets(int texSize,float deviation);

private:
	int m_KernelRaidius;
	float m_multiplier;
};







#endif