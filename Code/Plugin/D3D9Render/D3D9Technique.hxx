#include "D3D9Technique.h"

namespace ma
{
	D3D9Technique::D3D9Technique(IDirect3DDevice9 * pDevice)
	{
		m_pDXEffect = NULL;
		m_pD3D9Device = pDevice;
	}

	void D3D9Technique::Load(const char* pFileName, const char* pTechBaseName)
	{
		ASSERT(pTechBaseName);
		if (pTechBaseName == NULL)
			return;

		m_strTechName = pTechBaseName;

		std::string strPath; 
		if (pFileName == NULL)
		{
			strPath = std::string("../Data/Shader/D3D9/") + pTechBaseName + std::string(".fx");
		}
		else
		{
			strPath = pFileName;
		}

		DWORD dwShaderFlags = 0;
#ifdef _DEBUG
		dwShaderFlags |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif // _DEBUG

		HRESULT hr = D3D_OK;
		hr = D3DXCreateEffectFromFile( m_pD3D9Device, strPath.c_str(),
			NULL, NULL, dwShaderFlags, NULL, &m_pDXEffect, NULL ); 
		ASSERT(hr == D3D_OK);
	}

	void D3D9Technique::Begin(UINT uShaderFlg)
	{
		std::string strTechName = m_strTechName;
		if (uShaderFlg & ShderFLag_SKIN)
		{
			strTechName += "Skin";
		}

		m_pDXEffect->SetTechnique( strTechName.c_str() );

		HRESULT hr = D3D_OK;
		UINT cPasses = 0; 
		hr = m_pDXEffect->Begin(&cPasses, 0 );
		ASSERT(hr == D3D_OK);

		hr = m_pDXEffect->BeginPass(0);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9Technique::CommitChanges()
	{
		HRESULT hr = D3D_OK;
		hr = m_pDXEffect->CommitChanges();
		ASSERT(hr == D3D_OK);
	}

	void D3D9Technique::End()
	{
		HRESULT hr = D3D_OK;

		hr = m_pDXEffect->EndPass();
		ASSERT(hr == D3D_OK);

		hr = m_pDXEffect->End();	
		ASSERT(hr == D3D_OK);
	}
	
	void D3D9Technique::SetMatrixArray(const char* pParameter,Matrix4x4* ppMatrix, UINT Count) 
	{
		D3DXMATRIX arrMatrix[60];
		for (UINT i = 0; i < Count || i < 60; ++i)
		{
			Matrix4x4* pMatrix = &(ppMatrix[i]);
			arrMatrix[i] = D3DXMATRIX( (float*)pMatrix );
		}
		HRESULT hr = D3D_OK;
		hr = m_pDXEffect->SetMatrixArray(pParameter,arrMatrix,Count);
		ASSERT(hr == D3D_OK);
	}

	void D3D9Technique::SetMatrix(const char* pParameter,Matrix4x4* pMatrix)
	{
		HRESULT hr = D3D_OK;
		D3DXMATRIX matDx( (float*)pMatrix );
		hr = m_pDXEffect->SetMatrix(pParameter,&matDx);
		ASSERT(hr == D3D_OK);
	}

	void D3D9Technique::SetVector(const char* pParameter, Vector4* pVector)
	{
		HRESULT hr = D3D_OK;
		hr = m_pDXEffect->SetVector(pParameter,(const D3DXVECTOR4*)&pVector);
		ASSERT(hr == D3D_OK);
	}

	void D3D9Technique::SetTexture(const char* pParameter, Texture* pTexture)
	{
		HRESULT hr = D3D_OK;
		D3D9Texture* pD3D9Texture = (D3D9Texture*)pTexture;
		hr = m_pDXEffect->SetTexture(pParameter,pD3D9Texture->GetD3DTexture());
		ASSERT(hr == D3D_OK);
	}

}