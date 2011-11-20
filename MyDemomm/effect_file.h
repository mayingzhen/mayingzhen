#pragma once

#include "public.h"
#include "texture.h"


class CCamera;
class CMaterial;

class CEffectFile : public CRes
{
public:
	enum eMatrixHandles
	{
		k_worldMatrix = 0,
		k_viewMatrix,
		k_projMatrix,
		
		k_worldViewMatrix,
		k_viewProjMatrix,
		k_worldViewProjMatrix,
		
		k_worldMatrixArray,

		k_max_matrix_handles,
	};
		
	enum eParameterHandles
	{
		k_boneInfluenceCount=0,

		k_ambientMaterialColor,
		k_diffuseMaterialColor,
		k_emissiveMaterialColor,
		k_specularMaterialColor,
		k_specularMaterialPower,

		k_PointLightPos0,
		k_PointlightVec0,
		k_PointlightColor0,

		k_sunVector,
		k_sunColor,
		k_cameraPos,
		k_cameraDistances,
		k_cameraFacing,
		k_ambientLight,

		k_patchCorners,
		k_atmosphericLighting,

		k_posScaleOffset,
		k_uvScaleOffset,

		k_lensFlareColor,

		k_max_param_handles,
	};
	
 	enum eTextureHandles
 	{
 		k_max_texture_handles = 16,
 	};

public:
    CEffectFile();
    ~CEffectFile();

	bool destroyResource();					// destroy the resource
	bool disableResource();					// purge the resource from volatile memory
	bool restoreResource();					// prepare the resource for use (create any volatile memory objects needed)
	bool loadResource(const char* filename); // load the resource from a file (or NULL to use the resource name)

	bool begin() const;
	bool BeginPass(int nPass) const;
	bool BeginPass(const std::string& sPass) const;
	bool CommitChanges() const;
	bool endPass() const;
	void end() const;

	bool isParameterUsed(eParameterHandles index) const;
	bool isParameterUsed(const std::string& sDataName) const;
	bool isMatrixUsed(eMatrixHandles index) const;

	// Accessors...
	LPD3DXEFFECT effect()const;
	int totalPasses()const;

	bool setParameter(eParameterHandles index, const void* data, int size = D3DX_DEFAULT) const;
	//bool setParameter(std::string sParameter, const void* data, int valueSize) const;

	bool setMatrix(eMatrixHandles index, const D3DXMATRIX* data) const;
	//bool setMatrixArray(eMatrixHandles index, const D3DXMATRIX* data, UINT count) const;
	//bool setMatrixInArray(eMatrixHandles index, int element, const D3DXMATRIX* data) const;
	bool setFloatArray(eParameterHandles index, const float* data, int count) const;
	bool setFloatInArray(eParameterHandles index, int element, float data) const;
	//bool setTexture(int index, const CTexture* data) const;
	//bool setTextureMatrix(int index, const D3DXMATRIX* data) const;
	//bool setShadowTexture(const CTexture* data) const;
	bool SetTexture(const std::string& sTexture, IDirect3DTexture9* pD3DTexture) const;

	void applySurfaceMaterial(const CMaterial* pSurfaceMaterial) ;
	void applyCameraMatrices(const CCamera* pCamera) const;

private:

	enum 
	{
		MAX_PASS = 5,
	};

	// Private Data...
	LPD3DXEFFECT m_pEffect;
    D3DXEFFECT_DESC m_EffectDesc;
    D3DXHANDLE m_hTechnique;
	D3DXTECHNIQUE_DESC m_techniqueDesc;
	D3DXPASS_DESC m_passDesc[MAX_PASS];
	D3DXHANDLE m_pass[MAX_PASS];
    D3DXHANDLE m_paramHandle[k_max_param_handles];
    D3DXHANDLE m_matrixHandle[k_max_matrix_handles];
    D3DXHANDLE m_textureHandle[k_max_texture_handles];
    D3DXHANDLE m_textureMatrixHandle[k_max_texture_handles];
	D3DXHANDLE m_shadowTextureHandle;

	std::map<std::string, D3DXHANDLE> m_paramNameToHandle;

	UINT m_nNumPass;

	// Private Functions...
	void parseParameters();

	//CEffectFile(const CEffectFile& Src);
    //CEffectFile& operator=(const CEffectFile& Src);

};

extern const CEffectFile* g_pCurCEffect;

inline CEffectFile::CEffectFile()
:m_pEffect(0)
,m_shadowTextureHandle(0)
{
	memset(m_paramHandle, 0, sizeof(m_paramHandle));
	memset(m_matrixHandle, 0, sizeof(m_matrixHandle));
	memset(m_textureHandle, 0, sizeof(m_textureHandle));
	memset(m_textureMatrixHandle, 0, sizeof(m_textureMatrixHandle));
}

inline CEffectFile::~CEffectFile()
{
	//assert(!m_pEffect && "CEffectFile is still active");
	SAFE_RELEASE(m_pEffect);
}


inline LPD3DXEFFECT CEffectFile::effect() const
{
	return m_pEffect;
}

inline int CEffectFile::totalPasses()const
{
	assert(m_pEffect && "CEffectFile is not valid");
	return m_techniqueDesc.Passes;
}

inline bool CEffectFile::setParameter(eParameterHandles index, const void* data, int size)const
{
	if (m_pEffect && isParameterUsed(index))
	{
		bool result= SUCCEEDED(m_pEffect->SetValue(m_paramHandle[index], data, size));
		assert(result && "effect file error");
		return result;
	}
	return false;
}

// bool CEffectFile::setParameter(std::string sParameter, const void* data, int valueSize) const
// {
// 	//if (m_pEffect && is)
// 	return false;
// }

inline bool CEffectFile::setMatrix(eMatrixHandles index, const D3DXMATRIX* data)const
{
	if (m_pEffect && isMatrixUsed(index))
	{
		return SUCCEEDED(m_pEffect->SetMatrix(m_matrixHandle[index], data));
	}
	return false;
}

// inline bool CEffectFile::setMatrixArray(eMatrixHandles index, const D3DXMATRIX* data, UINT count)const
// {
// 	if (m_pEffect && isMatrixUsed(index))
// 	{
// 		return SUCCEEDED(m_pEffect->SetMatrixArray(m_matrixHandle[index], data, count));
// 	}
// 	return false;
// }

// inline bool CEffectFile::setMatrixInArray(eMatrixHandles index, int element, const D3DXMATRIX* data)const
// {
// 	if (m_pEffect && isMatrixUsed(index))
// 	{
// 		D3DXHANDLE subHandle = m_pEffect->GetParameterElement(m_matrixHandle[index],element);
// 		return SUCCEEDED(m_pEffect->SetMatrix(subHandle, data));
// 	}
// 	return false;
// }

inline bool CEffectFile::setFloatArray(eParameterHandles index, const float* data, int count)const
{
	if (m_pEffect && isParameterUsed(index))
	{
		return SUCCEEDED(m_pEffect->SetFloatArray(m_paramHandle[index], data, count));
	}
	return false;
}

inline bool CEffectFile::setFloatInArray(eParameterHandles index, int element, float data)const
{
	if (m_pEffect && isParameterUsed(index))
	{
		D3DXHANDLE subHandle = m_pEffect->GetParameterElement(m_paramHandle[index],element);
		return SUCCEEDED(m_pEffect->SetFloat(subHandle, data));
	}
	return false;
}

inline bool CEffectFile::SetTexture(const std::string& sTexture, IDirect3DTexture9* pD3DTexture) const
{
	if (m_pEffect)
	{
		std::map<std::string, D3DXHANDLE>::const_iterator it = m_paramNameToHandle.find(sTexture);
		if (it != m_paramNameToHandle.end() )
		{
			D3DXHANDLE textureHandle = it->second;	
			return SUCCEEDED(m_pEffect->SetTexture(textureHandle, pD3DTexture));
			//return SUCCEEDED(m_pEffect->SetTexture(sTexture.c_str(), pD3DTexture));
		}	
	}
	return false;
}


// inline bool CEffectFile::setTexture(int index, const CTexture* data) const
// {
// 	if ( m_pEffect && isTextureUsed(index) )
// 	{
// 		return SUCCEEDED(m_pEffect->SetTexture( m_textureHandle[index], data->getTexture() ) );
// 	}
// 	return false;
// }

// inline bool CEffectFile::setShadowTexture(const CTexture* data)const
// {
// 	if (m_pEffect && isShadowTextureUsed())
// 	{
// 		return SUCCEEDED(m_pEffect->SetTexture(m_shadowTextureHandle, data->getTexture()));
// 	}
// 	return false;
// }

// inline bool CEffectFile::setTextureMatrix(int index, const D3DXMATRIX* data)const
// {
// 	if (m_pEffect && isTextureMatrixUsed(index))
// 	{
// 		return SUCCEEDED(m_pEffect->SetMatrix(m_textureMatrixHandle[index], data));
// 	}
// 	return false;
// }


inline bool CEffectFile::isParameterUsed(eParameterHandles index)const
{
	return m_paramHandle[index] != 0;
}

inline bool CEffectFile::isMatrixUsed(eMatrixHandles index)const
{
	return m_matrixHandle[index] != 0;
}

inline bool CEffectFile::isParameterUsed(const std::string& sDataName) const
{
	return m_paramNameToHandle.find(sDataName) != m_paramNameToHandle.end();
}


