#include "public.h"
#include "effect_file.h"
#include "camera.h"
#include "Camera.h"
#include "Material.h"
#include "GameApp.h"


const CEffectFile* g_pCurCEffect = NULL;

bool CEffectFile::destroyResource()
{
	SAFE_RELEASE(m_pEffect);
	return true;
}


bool CEffectFile::disableResource()
{
	if (m_pEffect)
		m_pEffect->OnLostDevice();
	
	return true;
}

bool CEffectFile::restoreResource()
{
	if (m_pEffect)
		m_pEffect->OnResetDevice();
	
	return true;
}

bool CEffectFile::loadResource(const char* filename)
{
	SAFE_RELEASE(m_pEffect);

	char fullPath[MAX_PATH];

	_tfullpath(fullPath, filename, MAX_PATH);

	DWORD nFlags = 0;
#ifdef _DEBUG
	nFlags |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif // _DEBUG
	
    LPD3DXBUFFER pBufferErrors = NULL;
    HRESULT result = D3DXCreateEffectFromFile(g_pD3DDevice, fullPath, NULL, NULL, nFlags, NULL, &m_pEffect, &pBufferErrors);
    if( FAILED(result) )
	{
		ASSERTEX1( false, "%s", (char*)pBufferErrors->GetBufferPointer() );
		SAFE_RELEASE(pBufferErrors);
        return false;
	}
	SAFE_RELEASE(pBufferErrors);

	m_pEffect->GetDesc(&m_EffectDesc); 

	result = m_pEffect->FindNextValidTechnique(NULL, &m_hTechnique); 
    if( FAILED(result) )
	{
		assert(false);
		SAFE_RELEASE(m_pEffect);
        return false;
	}

	result = m_pEffect->GetTechniqueDesc(m_hTechnique, &m_techniqueDesc);
    if( FAILED(result) )
	{
		assert(false);
		SAFE_RELEASE(m_pEffect);
        return false;
	}

	int index = 0;
	while ( index < MAX_PASS )
	{	
		D3DXHANDLE passHandle = m_pEffect->GetPass(m_hTechnique,index); 
		if (passHandle == NULL)
			break;

		m_pass[index] = passHandle;
		result = m_pEffect->GetPassDesc(passHandle, &m_passDesc[index]);
		++index;
	}
	
	parseParameters();

	return true;
}


bool CEffectFile::begin() const
{
	assert(m_pEffect);
	if (m_pEffect == NULL)
		return false;

	HRESULT result ;
	
	result = m_pEffect->SetTechnique(m_hTechnique);

	g_pCurCEffect = this;

	UINT nPass;
	result = m_pEffect->Begin(&nPass, 0/*D3DXFX_DONOTSAVESTATE|D3DXFX_DONOTSAVESHADERSTATE|D3DXFX_DONOTSAVESAMPLERSTATE*/);
    if( FAILED( result ) )
	{
		assert(false);
        return false;
	}

	// set the lighting parameters
	// from the global light manager

	// set the camera matricies
	//applyCameraMatrices( /*TheGameHost.activeCamera()*/&g_Camera );

    return true;
}

bool CEffectFile::BeginPass(int nPass) const
{
	HRESULT result = m_pEffect->BeginPass(nPass);
    if( FAILED(result) )
	{
		assert(false);
        return false;
	}

    return true;
}

bool CEffectFile::BeginPass(std::string sPass) const
{
	for (int i = 0; i < MAX_PASS; ++i)
	{
		if ( m_passDesc[i].Name == sPass)
		{
			HRESULT result = m_pEffect->BeginPass(i);
			if( FAILED(result) )
			{
				assert(false);
				return false;
			}
			return true;
		}
	}
	assert(false);
	return false;
}



bool CEffectFile::CommitChanges() const
{
	HRESULT result = m_pEffect->CommitChanges();
    if( FAILED(result) )
	{
		assert(false);
        return false;
	}
    return true;
}

bool CEffectFile::endPass() const
{
	HRESULT result = m_pEffect->EndPass();
	if( FAILED(result) )
	{
		assert(false);
        return false;
	}
    return true;
}

void CEffectFile::end() const
{
	assert(m_pEffect);
	if (m_pEffect == NULL)
		return;

	HRESULT result = m_pEffect->End();
    if( FAILED(result) )
	{
		assert(false);
	}

	g_pCurCEffect = NULL;
}


void CEffectFile::parseParameters()
{
    // Look at parameters for semantics and annotations that we know how to interpret
    D3DXPARAMETER_DESC ParamDesc;
    D3DXHANDLE hParam;
	//static char numerals[] = {'0','1','2','3','4','5','6','7','8','9'};
	static std::string numerals = "0123456789";

	memset( m_paramHandle, 0, sizeof(m_paramHandle) );
	memset( m_matrixHandle, 0, sizeof(m_matrixHandle) );
	memset( m_textureHandle, 0, sizeof(m_textureHandle) );
	memset( m_textureMatrixHandle, 0, sizeof(m_textureMatrixHandle) );

    for( UINT iParam = 0; iParam < m_EffectDesc.Parameters; iParam++ )
    {
        hParam = m_pEffect->GetParameter ( NULL, iParam );
        m_pEffect->GetParameterDesc( hParam, &ParamDesc );
        if( ParamDesc.Semantic != NULL && 
            ( ParamDesc.Class == D3DXPC_MATRIX_ROWS || ParamDesc.Class == D3DXPC_MATRIX_COLUMNS ) )
        {
            if( strcmpi( ParamDesc.Semantic, "world" ) == 0 )
                m_matrixHandle[k_worldMatrix] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "view" ) == 0 )
                m_matrixHandle[k_viewMatrix] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "projection" ) == 0 )
                m_matrixHandle[k_projMatrix] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "worldview" ) == 0 )
                m_matrixHandle[k_worldViewMatrix] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "viewprojection" ) == 0 )
                m_matrixHandle[k_viewProjMatrix] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "worldviewprojection" ) == 0 )
                m_matrixHandle[k_worldViewProjMatrix] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "worldmatrixarray" ) == 0 )
                m_matrixHandle[k_worldMatrixArray] = hParam;

			// look for texture matrices which are named texMatX
			if (strnicmp( ParamDesc.Semantic, "texmat", 6) ==0)
			{
				std::string name(ParamDesc.Name);
				size_t iPos = name.find_first_of(numerals,0);
				if (iPos != std::string::npos)
				{
					int iTexture = atoi(&ParamDesc.Name[iPos]);
					if (iTexture>=0 && iTexture<k_max_texture_handles)
					{
						m_textureMatrixHandle[iTexture] = hParam;
					}
				}
			}
        }

        if( ParamDesc.Semantic != NULL && 
            ( ParamDesc.Class == D3DXPC_VECTOR ))
        {
            if( strcmpi( ParamDesc.Semantic, "materialambient" ) == 0 )
                m_paramHandle[k_ambientMaterialColor] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "materialdiffuse" ) == 0 )
                m_paramHandle[k_diffuseMaterialColor] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "materialspecular" ) == 0 )
                m_paramHandle[k_specularMaterialColor] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "materialemissive" ) == 0 )
                m_paramHandle[k_emissiveMaterialColor] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "sunvector" ) == 0 )
                m_paramHandle[k_sunVector] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "suncolor" ) == 0 )
                m_paramHandle[k_sunColor] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "worldcamerapos" ) == 0 )
                m_paramHandle[k_cameraPos] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "viewdistances" ) == 0 )
                m_paramHandle[k_cameraDistances] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "worldviewvector" ) == 0 )
                m_paramHandle[k_cameraFacing] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "ambientlight" ) == 0 )
                m_paramHandle[k_ambientLight] = hParam;

            else if( strcmpi( ParamDesc.Semantic, "posScaleOffset" ) == 0 )
                m_paramHandle[k_posScaleOffset] = hParam;
            else if( strcmpi( ParamDesc.Semantic, "uvScaleOffset" ) == 0 )
                m_paramHandle[k_uvScaleOffset] = hParam;

            else if( strcmpi( ParamDesc.Semantic, "flareColor" ) == 0 )
                m_paramHandle[k_lensFlareColor] = hParam;
			
       }

        if(ParamDesc.Class == D3DXPC_SCALAR)
        {
			if( ParamDesc.Semantic == NULL)
			{
				if( strcmpi( ParamDesc.Name, "curnumbones" ) == 0 )
				{
					m_paramHandle[k_boneInfluenceCount] = hParam;
				}
			}
			else
			{
				if( strcmpi( ParamDesc.Semantic, "materialpower" ) == 0 )
					m_paramHandle[k_specularMaterialPower] = hParam;
			}
		}

        if( ParamDesc.Class == D3DXPC_OBJECT )
        {
			std::string name(ParamDesc.Name);
			
			if (ParamDesc.Type == D3DXPT_TEXTURE
				&& name == "shadow")
			{
				m_shadowTextureHandle = hParam;
			}
			else if ( ParamDesc.Type == D3DXPT_TEXTURE || 
					  ParamDesc.Type == D3DXPT_TEXTURE2D || 
					  ParamDesc.Type == D3DXPT_TEXTURE3D || 
					  ParamDesc.Type == D3DXPT_TEXTURECUBE )
			{
				size_t iPos = name.find_first_of(numerals,0);
				if (iPos != std::string::npos)
				{
					int iTexture = atoi(&ParamDesc.Name[iPos]);
					if (iTexture>=0 && iTexture<k_max_texture_handles)
					{
						m_textureHandle[iTexture] = hParam;
					}
				}
			}
        }

		if ( ParamDesc.Class == D3DXPC_STRUCT)
		{
			if( strcmpi( ParamDesc.Semantic, "AtmosphericLighting" ) == 0 )
				m_paramHandle[k_atmosphericLighting] = hParam;
			else if ( strcmpi( ParamDesc.Semantic, "patchCorners") == 0 )
				m_paramHandle[k_patchCorners] = hParam;
		}
	}
}


void CEffectFile::applySurfaceMaterial(const CMaterial* pSurfaceMaterial)
{
	if (pSurfaceMaterial == NULL)
		return;

	const D3DMATERIAL9& d3dMaterial = pSurfaceMaterial->GetD3dMaterial();
	if ( isParameterUsed(k_ambientMaterialColor) )
		setParameter(k_ambientMaterialColor, &d3dMaterial.Ambient);

	if ( isParameterUsed(k_diffuseMaterialColor) )
		setParameter(k_diffuseMaterialColor, &d3dMaterial.Diffuse);
	
	if ( isParameterUsed(k_specularMaterialColor) )
		setParameter(k_specularMaterialColor, &d3dMaterial.Specular);

	if ( isParameterUsed(k_emissiveMaterialColor) )
		setParameter(k_specularMaterialColor, &d3dMaterial.Emissive);

	if ( isParameterUsed(k_specularMaterialPower) )
		setParameter(k_specularMaterialPower, &d3dMaterial.Power);

	// set textures
	const std::vector<CTexture*>& vecTexLayer = pSurfaceMaterial->GetTexLayer();
	for (int i = 0; i < vecTexLayer.size(); ++i)
	{
		CTexture* texLayer = vecTexLayer[i];
		if ( texLayer && texLayer->IsValid() )
			m_pEffect->SetTexture( m_textureHandle[i],texLayer->GetD3DTexture() );

		if ( texLayer->IsUseMatrix() )
			m_pEffect->SetMatrix( m_textureMatrixHandle[i], texLayer->GetTexMatrix() );
	}
}


void CEffectFile::applyCameraMatrices(const CCamera* pCamera) const
{
	if (pCamera)
	{
		setMatrix( k_viewMatrix, &pCamera->m_mView );
		setMatrix( k_projMatrix, &pCamera->m_mProj );
		setMatrix( k_viewProjMatrix, &( pCamera->m_mView * pCamera->m_mProj ) );
		setParameter( k_cameraPos, &pCamera->m_vEyePt);
		setParameter( k_cameraFacing, &pCamera->m_vLookAtPt );

		//cVector4 camDistances(
		//	pCamera->nearPlane(),
		//	pCamera->farPlane(),
		//	pCamera->farPlane()-pCamera->nearPlane(),
		//	1.0f/pCamera->farPlane());
			
		//setParameter(k_cameraDistances, &camDistances);
	}
}
