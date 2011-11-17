#include "sky.h"
#include "Material.h"
#include "effect_file.h"
#include "TextureMngr.h"
#include "Camera.h"
#include "GameApp.h"


CSky::CSky()
{
	m_pMesh = 0;
	//memset( m_texture, 0, sizeof(CTexture*) );
	//NumSubsets = 6;
}

CSky::~CSky()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pSkyMap);
// 	for (int i = 0; i < 6; ++i)
// 	{
// 		if (m_texture[i])
// 			m_texture[i]->Release();
// 	}
}

void CSky::Create()
{
	HRESULT hr = 0;

	m_SkyBoxModel.LoadFromXFile("res\\skybox01.x");

	hr = D3DXCreateCubeTextureFromFile(g_pD3DDevice,"res\\skybox02.dds",&m_pSkyMap);

	m_SkyTech.loadResource("shader\\simple_skybox.fx");	

/*
	hr = D3DXCreateMeshFVF(12,24,D3DXMESH_MANAGED,Vertex::FVF,g_pD3DDevice,&m_pMesh);

	if(FAILED(hr))
	{
		::MessageBox(0, "D3DXCreateMeshFVF() - FAILED", 0, 0);
		return ;
	}


	Vertex* v = 0;
	m_pMesh->LockVertexBuffer(0, (void**)&v); //顶点列表

	v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[1] = Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[2] = Vertex( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[3] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

	v[4] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[5] = Vertex( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[6] = Vertex( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[7] = Vertex(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

	v[8]  = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	v[9]  = Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[10] = Vertex( 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[11] = Vertex( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	v[12] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	v[14] = Vertex( 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

	v[16] = Vertex(-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[17] = Vertex(-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[18] = Vertex(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[19] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	v[20] = Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[21] = Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[22] = Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[23] = Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    
	m_pMesh->UnlockVertexBuffer();

	WORD* i = 0;
	m_pMesh->LockIndexBuffer(0, (void**)&i);  //顶点索引列表

	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	m_pMesh->UnlockIndexBuffer();

	DWORD* attributeBuffer = 0;
	m_pMesh->LockAttributeBuffer(0, &attributeBuffer);

	for(int a = 0; a < 2; a++)
		attributeBuffer[a] = 0;

	for(int b = 2; b < 4; b++)
		attributeBuffer[b] = 1;

	for(int c = 4; c < 6; c++)
		attributeBuffer[c] = 2;

	for(int d = 6; d < 8; d++)
		attributeBuffer[d] = 3;

	for(int e = 8; e < 10; e++)
		attributeBuffer[e] = 4;

	for(int f = 10; f < 12; f++)
		attributeBuffer[f] = 5;

	m_pMesh->UnlockAttributeBuffer();

	DWORD *adjacencyBuffer;
	adjacencyBuffer = new DWORD[m_pMesh->GetNumFaces() * 6];
	m_pMesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]);

	hr = m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_COMPACT|
		D3DXMESHOPT_VERTEXCACHE,&adjacencyBuffer[0],0, 0, 0);

	m_texture[0] = (CTexture*)TextureMngr()->Query("res\\sky\\1 front.bmp");
	m_texture[1] = (CTexture*)TextureMngr()->Query("res\\sky\\3 back.bmp");
	m_texture[2] = (CTexture*)TextureMngr()->Query("res\\sky\\6 up-lt180.bmp");
	m_texture[3] = (CTexture*)TextureMngr()->Query("res\\sky\\5 down.bmp");
	m_texture[4] = (CTexture*)TextureMngr()->Query("res\\sky\\2 left.bmp");
	m_texture[5] = (CTexture*)TextureMngr()->Query("res\\sky\\4 right.bmp");
	*/
}


void CSky::Render()
{	
	m_SkyTech.begin();
	m_SkyTech.BeginPass(0);

	LPD3DXEFFECT effect = m_SkyTech.effect();

	D3DXMATRIX matWorld,matScla;
	D3DXMatrixScaling(&matScla, 120,120,120); // box 大小 = 100 * 2
	D3DXMatrixTranslation(&matWorld,0,0,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matScla);

	//if ( m_pSkyMap && m_pSkyMap->IsValid() )
	//{
		V( effect->SetTexture( "skyCubeMapTex", m_pSkyMap ) );
	//}

	//V( effect->SetMatrix( "matWorld", &matWorld ) );

	//m_SkyBoxModel.Upadte();

	m_SkyBoxModel.Render(&matWorld);

	m_SkyTech.endPass();
	m_SkyTech.end();


	/*
 	D3DXMATRIX matWorld,matScla;
 	D3DXMatrixScaling(&matScla, 5000,5000,5000);
 	D3DXMatrixTranslation(&matWorld,0,0,0);
 	D3DXMatrixMultiply(&matWorld,&matWorld,&matScla);
// 	g_pD3DDevice->SetTransform(D3DTS_WORLD,&matWorld);
// 
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
 	g_pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
 	g_pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);


	if (g_pCurCEffect)
	{
		//D3DXMATRIX mv  = pFrame->CombinedTransformationMatrix * g_Camera.m_mView;
		//D3DXMATRIX mvp = mv * g_Camera.m_mProj;
		D3DXMATRIX matView,matProj;
		g_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION,&matProj);
		//matView = g_Camera.m_mView;
		//matProj = g_Camera.m_mProj;
		D3DXMATRIX matWorldView = matWorld* matView;
		D3DXMATRIX matWorldViewProj = matWorldView * matProj;
		g_pCurCEffect->setMatrix(CEffectFile::k_worldViewMatrix,&matWorldView);
		g_pCurCEffect->setMatrix(CEffectFile::k_worldViewProjMatrix,&matWorldViewProj);
	}
	else
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	}

	for(int i = 0; i < 6; i++)
	{
		CTexture* tempTex = m_texture[i];
		if ( tempTex && tempTex->IsValid() )
		{
			if ( g_pCurCEffect && g_pCurCEffect->effect() )
				g_pCurCEffect->effect()->SetTexture( "g_TextureSrcDiffuse", tempTex->GetD3DTexture() );
			else
				g_pD3DDevice->SetTexture( 0, tempTex->GetD3DTexture() );
		}

		// if set effect parm,must be use before every DrawIndexedPrimitive
		if (g_pCurCEffect)
			g_pCurCEffect->CommitChanges(); 

		m_pMesh->DrawSubset(i);
	}
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	g_pD3DDevice->SetTexture( 0, NULL );
	*/
 }




// void CSky::Create()
// {
// 	char* modelPath = "sky\\unit_dome.x";
// 	DWORD dwNumMaterials = 0;
// 	SAFE_RELEASE(m_pMesh);
// 	if( FAILED( D3DXLoadMeshFromX(modelPath,D3DXMESH_MANAGED,g_pD3DDevice, 
// 		NULL,NULL,NULL,&dwNumMaterials,&m_pMesh) ) )
// 	{
// 		assert(false);
// 	}
// 
// 	CEffectFile* effectFile = new CEffectFile;
// 	effectFile->loadResource("sky\\simple_skydome.fx");
// 	CTexture* textureSky = (CTexture*)TextureMngr()->Query("sky\\sky_box2.dds");
// 	CTexture* textureClouds = (CTexture*)TextureMngr()->Query("sky\\clouds.dds");
// 	m_pMaterial = new CMaterial;
// 	m_pMaterial->SetEffectFile(effectFile);
// 	m_pMaterial->SetTexture(textureSky);
// 	m_pMaterial->SetTexture(textureClouds);
// 	m_pMaterial->SetScenBlend(CMaterial::FIRE_BLEND);
// 	m_pMaterial->SetFileMode(D3DFILL_WIREFRAME);
// 
// 	m_uvScaleOffset = D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f);
// }







