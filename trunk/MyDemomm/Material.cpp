#include "Material.h"
#include "Texture.h"
#include "effect_file.h"

CMaterial::CMaterial()
{
	m_eBlendMode = SOLID;
	m_eFileMode = D3DFILL_SOLID;
	m_bAlphaTest = false;  
	m_bFog = false;		
	m_bCullNone = false;   

	m_pEffectFile = NULL;

	//m_lpMaterial.Ambient = D3DCOLOR_COLORVALUE(1.0,1.0,1.0,1.0);
	//m_AmbientColor=  GEColor::White;
	//m_DiffuseColor=GEColor::White;
	//m_SpecularColor=GEColor::White;
	//m_fPower=0.0f;
	//m_EmissiveColor=GEColor::Black;
}

CMaterial::~CMaterial()
{
}


void CMaterial::BeginRender(IDirect3DDevice9 *lpDevice) 
{
	//lpDevice->SetMaterial(&m_lpMaterial);

//  	if (m_bCullNone)
//  		lpDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
//  	else
//  		lpDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

	//lpDevice->SetRenderState(D3DRS_FILLMODE,m_eFileMode);
	
	if (m_bFog)
		lpDevice->SetRenderState(D3DRS_FOGENABLE,TRUE);
	else
		lpDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);
	
	switch(m_eBlendMode)
	{
	case SOLID:
		lpDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
		lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
		break;
	case GLASS_BLEND:
		lpDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
		lpDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
		lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		lpDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		lpDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);			
		break;
	case FIRE_BLEND:
		lpDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
		lpDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
		lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		lpDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		lpDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
		break;
	case DARK_BLEND:
		lpDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT );
		lpDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
		lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		lpDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		lpDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
		break;
	case DARK2_BLEND:
		lpDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
		lpDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
		lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		lpDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVDESTALPHA);
		lpDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		break;
	case NONAME_BLEND:
		lpDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD );
		lpDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
		lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		lpDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO );
		lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
		break;
	default:
		break ;
	}

	if ( m_vecTexLayer.size() > 0 )
	{
		for (int i = 0; i < (int)m_vecTexLayer.size(); i++)
		{
			CTexture* tempTex = m_vecTexLayer[i];
			if ( !tempTex || !tempTex->IsValid() )
				continue;
			
			lpDevice->SetTexture( i, tempTex->GetD3DTexture() );	

			lpDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
			lpDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
			lpDevice->SetSamplerState(i,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
			lpDevice->SetSamplerState(i,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
			lpDevice->SetSamplerState(i,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
	
			lpDevice->SetTextureStageState( i,D3DTSS_COLOROP, tempTex->GetTexOp() );
			lpDevice->SetTextureStageState(i,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			lpDevice->SetTextureStageState(i,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			lpDevice->SetTextureStageState(i,D3DTSS_COLORARG2,D3DTA_CURRENT);
			lpDevice->SetTextureStageState(i,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_PASSTHRU);					
		
			if ( tempTex->IsUseMatrix() )
			{
				lpDevice->SetTextureStageState(i,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);					
				lpDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + i),tempTex->GetTexMatrix() );
			}
		}	
	}
	else
	{
		lpDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG2);
		lpDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG2);		
	}	

	lpDevice->SetRenderState(D3DRS_LIGHTING,FALSE);	

	if (m_bAlphaTest)
	{
		lpDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
		lpDevice->SetRenderState(D3DRS_ALPHAREF,10);
	}
	
 	if ( m_pEffectFile /*&& m_pEffectFile->IsValid()*/ )
 	{
		m_pEffectFile->begin();
		m_pEffectFile->applySurfaceMaterial(this);
	}
 	else
 	{
 		lpDevice->SetVertexShader(NULL);
		lpDevice->SetPixelShader(NULL);	
 	}
}

void CMaterial::EndRender(IDirect3DDevice9 * lpDevice)  
{	
 	if ( m_pEffectFile /*&& m_pEffectFile->IsValid()*/ )
	{
 		m_pEffectFile->end();
 	}

	lpDevice->SetRenderState(D3DRS_FILLMODE,SOLID);

	switch(m_eBlendMode)
	{
	case SOLID:		
		
		break;
	case GLASS_BLEND:
		lpDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
		lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);	
		break;
	case FIRE_BLEND:
		lpDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
		lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);			
		break;
	case DARK_BLEND:
		lpDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
		lpDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
		lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
		break;
	case DARK2_BLEND:
		lpDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
		lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
		break;
	case NONAME_BLEND:
		lpDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
		lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
		break;
	default:
		return ;
	}
	
	if ( m_vecTexLayer.size() > 0 )
	{
		for (int i = 0; i < (int)m_vecTexLayer.size(); i++)
		{						
			if (i == 0)
				lpDevice->SetTextureStageState(i,D3DTSS_COLOROP,D3DTOP_MODULATE);
			else
				lpDevice->SetTextureStageState(i,D3DTSS_COLOROP,D3DTOP_DISABLE);
		
			lpDevice->SetTextureStageState(i,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			lpDevice->SetTextureStageState(i,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);						
		}
	}

	lpDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
	lpDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);
	lpDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
}



bool CMaterial::operator < (const CMaterial &data) const
{
	if ( (int)m_eBlendMode < (int)data.m_eBlendMode) 
		return true;
	else if ( (int)m_eBlendMode > (int)data.m_eBlendMode) 
		return false;

	if (m_strEffectName < data.m_strEffectName) 
		return true;
	else if (m_strEffectName > data.m_strEffectName) 
		return false;

	if ( m_vecTexLayer.size() < data.m_vecTexLayer.size() ) 
		return true;
	else if ( m_vecTexLayer.size() > data.m_vecTexLayer.size() ) 
		return false;

	for (int i = 0; i < (int)m_vecTexLayer.size();i++)
	{
		if ( m_vecTexLayer[i]->GetName() < data.m_vecTexLayer[i]->GetName() ) 
			return true;
		else if ( m_vecTexLayer[i]->GetName()  > data.m_vecTexLayer[i]->GetName() ) 
			return false;
	}

	if ( (int)m_bFog < (int)data.m_bFog ) 
		return true;
	else if ( (int)m_bFog < (int)data.m_bFog) 
		return false;

	if ( (int)m_bAlphaTest < (int)data.m_bAlphaTest ) 
		return true;
	else if ( (int)m_bAlphaTest > (int)data.m_bAlphaTest ) 
		return false;
/*
	for (int i = 0; i < (int)m_vecTexLayer.size();i++)
	{
		CTexture* texture = m_vecTexLayer[i];

		if ( (int)texture->IsUseMatrix() < (int)data.m_vecUseMatrix[i] ) 
			return true;
		else if ( (int)m_vecUseMatrix[i] > (int)data.m_vecUseMatrix[i] ) 
			return false;

		if (m_vecUseMatrix[i])
		{
			const FLOAT *fVal0 = (const FLOAT *)m_vecTextureMatrix[i];
			const FLOAT *fVal1 = (const FLOAT *)data.m_vecTextureMatrix[i];
			for (int j = 0; j < 16; j++)
			{
				if (fVal0[j] < fVal1[j]) return true;
				else if (fVal0[j] > fVal1[j]) return false;
			}
		}
	}
*/
	
	return false;
}

// if (m_iPriority < data.m_iPriority) return true;
// else if (m_iPriority > data.m_iPriority) return false;
// if ((int)m_eBlendMode < (int) data.m_eBlendMode) return true;
// else 
// {
// 	if (m_eBlendMode > (int)data.m_eBlendMode) return false;
// }
// if (m_strEffectName < data.m_strEffectName) 
// {
// 	return true;
// }
// else
// {
// 	if (m_strEffectName > data.m_strEffectName) return false;
// }
// 
// if (m_vecTexInfo.size() < data.m_vecTexInfo.size()) return true;
// else
// {
// 	if (m_vecTexInfo.size() > data.m_vecTexInfo.size()) return false;
// }
// if (m_vecTexInfo.size() < data.m_vecTexInfo.size()) return true;
// else if (m_vecTexInfo.size() > data.m_vecTexInfo.size()) return false;
// for (int i = 0; i < (int)m_vecTexInfo.size();i++)
// {
// 	if (m_vecTexInfo[i]->m_strTexName < data.m_vecTexInfo[i]->m_strTexName) return true;
// 	else if (m_vecTexInfo[i]->m_strTexName > data.m_vecTexInfo[i]->m_strTexName) return false;
// 	if (m_vecTexInfo[i]->m_strTexParamName < data.m_vecTexInfo[i]->m_strTexParamName) return true;
// 	else if (m_vecTexInfo[i]->m_strTexParamName > data.m_vecTexInfo[i]->m_strTexParamName) return false;
// 
// }
// 
// if ((int)m_bFog < (int)data.m_bFog) return true;
// else
// {
// 	if ((int)m_bFog < (int)data.m_bFog) return false;
// }
// if ((int)m_bAlphaTest < (int)data.m_bAlphaTest) return true;
// else
// {
// 	if ((int)m_bAlphaTest > (int)data.m_bAlphaTest) return false;
// }
// ASSERT(m_vecTexturePtr.size() == data.m_vecTexturePtr.size());
// for (int i = 0; i < (int)m_vecTexturePtr.size();i++)
// {			
// 	if ((int)m_vecUseMatrix[i] < (int)data.m_vecUseMatrix[i]) return true;
// 	else if ((int)m_vecUseMatrix[i] > (int)data.m_vecUseMatrix[i]) return false;
// 	if (m_vecUseMatrix[i])
// 	{
// 		const FLOAT *fVal0 = (const FLOAT *)m_vecTextureMatrix[i];
// 		const FLOAT *fVal1 = (const FLOAT *)data.m_vecTextureMatrix[i];
// 		for (int j = 0; j < 16; j++)
// 		{
// 			if (fVal0[j] < fVal1[j]) return true;
// 			else if (fVal0[j] > fVal1[j]) return false;
// 		}
// 	}
// }
// return false;




