#include "TerrainLiquid.h"
#include "Terrain.h"


namespace ma
{
	const int MAIN_BINDING = 0;

	TerrainLiquid::TerrainLiquid()
	{
		m_bDepthEnable = false;
		m_fDepthScale = 0.0f;
		mfLastUpdate = 0;


		//m_pVertex = NULL;
		//m_pIndex = NULL;
		//m_pVertexDeclaration = 0;

		m_ReflectionTexture = NULL;
		m_RefractionTexture = NULL;

		m_pDepthTex = NULL;

		m_pSubMeshData = new SubMeshData();
	}

	TerrainLiquid::~TerrainLiquid()
	{
		//SAFE_RELEASE(m_pVertexDeclaration);
		//SAFE_RELEASE(m_pVertex);
		//SAFE_RELEASE(m_pIndex);
		//SAFE_RELEASE(m_pDepthTex);
		//for(int i = 0; i < m_allFrameTex.size(); ++i)
		//{
			//SAFE_RELEASE(m_allFrameTex[i]);
		//}

		//SAFE_RELEASE(m_ReflectionTexture);
		//SAFE_RELEASE(m_RefractionTexture);
	}


	void TerrainLiquid::create(const Vector3& seedPos, 
		float fTextureScale, bool bDepthEnable, 
		float fDepthScale, Terrain* pTerrainSystem )
	{
		// init parameter
		mSeedPos = seedPos;
		mStep = Vector2(pTerrainSystem->m_scale.x, pTerrainSystem->m_scale.z);
		m_pTerrainSystem = pTerrainSystem;
		m_fTextureScale = fTextureScale;
		m_bDepthEnable = bDepthEnable;
		m_fDepthScale = fDepthScale;

		// seed fill
		spreed( 0,0,FOUR_DIR );

		// create vertex & index
		createVertexData();
		createIndexData();
		createVertexDeclation();

		CreateWaterTex();
	}

// 	void TerrainLiquid::BuildReflectionTexture()
// 	{
// 		HRESULT hr = S_OK;
// 
// 		// Get the current view and projection matrix
// 		Matrix4x4 OriginalViewMatrix = g_Camera.m_mView;
// 		Matrix4x4 OriginalProjectionMatrix = g_Camera.m_mProj;
// 
// 		// Set mirror camera
// 		D3DXMATRIX MirrorViewMatrix;
// 		//MirrorViewMatrix = OriginalViewMatrix;
// 		D3DXMATRIX TrafoMatrix;
// 		D3DXMatrixIdentity(&TrafoMatrix);
// 		TrafoMatrix._22 = -1.0f;
// 		TrafoMatrix._42 = 2.0f * mSeedPos.y;//m_fWaterHeight;
// 		MirrorViewMatrix = TrafoMatrix * OriginalViewMatrix;
// 		g_pD3DDevice->SetTransform(D3DTS_VIEW,&MirrorViewMatrix);
// 
// 		D3DXMATRIX MirrorProjectionMatrix;
// 		MirrorProjectionMatrix = OriginalProjectionMatrix;
// 		g_pD3DDevice->SetTransform(D3DTS_PROJECTION,&MirrorProjectionMatrix);
// 
// 		LPDIRECT3DSURFACE9 pOldRT0 = NULL;
// 		g_pD3DDevice->GetRenderTarget(0, &pOldRT0);
// 
// 		LPDIRECT3DSURFACE9 pSurfReflection = NULL;
// 		hr = m_ReflectionTexture->GetSurfaceLevel(0,&pSurfReflection);
// 		hr = g_pD3DDevice->SetRenderTarget(0,pSurfReflection);
// 		SAFE_RELEASE(pSurfReflection);
// 
// 		// Get the current viewport
// 		D3DVIEWPORT9 OriginalViewport;
// 		g_pD3DDevice->GetViewport(&OriginalViewport);
// 
// 		// Set new viewport
// 		D3DVIEWPORT9 CameraViewport;
// 		CameraViewport.X		= 0;
// 		CameraViewport.Y		= 0;
// 		CameraViewport.Width	= g_nWndWidth;
// 		CameraViewport.Height	= g_nWndHeigh;
// 		CameraViewport.MinZ		= 0.0f;
// 		CameraViewport.MaxZ		= 1.0f;
// 		g_pD3DDevice->SetViewport(&CameraViewport);
// 
// 		g_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
// 			D3DCOLOR_RGBA(0, 0, 0, 0),1.0f,0);
// 
// 		// Enable clip plane
// 		//g_pD3DDevice->SetClipPlane( 1, true );
// 		D3DXPLANE ClipPlane;
// 		ClipPlane.a	= 0.0f;
// 		ClipPlane.b	= 1.0f;
// 		ClipPlane.c	= 0.0f;
// 		ClipPlane.d	= -mSeedPos.y;
// 		g_pD3DDevice->SetClipPlane( 1, ClipPlane );
// 
// 		// render to RT
// 		//g_SceneMng.Render();
// 		//Terrain::GetSingleton().Render();
// 		//CSky::GetSingleton().Render();
// 
// 		//g_pD3DDevice->SetClipPlane(0,NULL);
// 		g_pD3DDevice->SetRenderTarget(0,pOldRT0);
// 		g_pD3DDevice->SetViewport(&OriginalViewport);
// 	}
// 
// 	void TerrainLiquid::BuildRefractionTexture()
// 	{
// 		HRESULT hr = S_OK;
// 
// 		LPDIRECT3DSURFACE9 pOldRT0 = NULL;
// 		g_pD3DDevice->GetRenderTarget(0, &pOldRT0);
// 
// 		LPDIRECT3DSURFACE9 pSurfRefraction = NULL;
// 		hr = m_RefractionTexture->GetSurfaceLevel(0,&pSurfRefraction);
// 		hr = g_pD3DDevice->SetRenderTarget(0,pSurfRefraction);
// 		SAFE_RELEASE(pSurfRefraction);
// 
// 		g_pD3DDevice->Clear(
// 			0,
// 			NULL,
// 			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
// 			D3DCOLOR_RGBA(0, 0, 0, 0),
// 			1.0f,
// 			0);
// 
// 	}

	void TerrainLiquid::CreateWaterTex()
	{
		//HRESULT hr = D3D_OK;

// 		m_allFrameTex.resize(24);
// 		for (int i = 0; i < 24; ++i)
// 		{
// 			char sFrameName[MAX_PATH] = {0};
// 			_snprintf( sFrameName, sizeof(sFrameName), "res\\terrain\\jpg_riverWater_%d.dds", i  );
// 			D3DXCreateTextureFromFile( g_pD3DDevice, sFrameName, &(m_allFrameTex[i]) );
// 		}
// 
// 		D3DXCreateTextureFromFile( g_pD3DDevice, "res\\terrain\\water_depth.tga", &m_pDepthTex );
// 
// 		hr = g_pD3DDevice->CreateTexture( g_nWndWidth, g_nWndHeigh, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
// 			D3DPOOL_DEFAULT, &m_ReflectionTexture, NULL);
// 
// 		hr = g_pD3DDevice->CreateTexture( g_nWndWidth, g_nWndHeigh, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
// 			D3DPOOL_DEFAULT, &m_RefractionTexture, NULL);
	}


	void TerrainLiquid::createVertexDeclation()
	{
		m_pDeclaration = GetRenderDevice()->CreateVertexDeclaration();
		m_pDeclaration->AddElement(0,0,DT_FLOAT3,DU_POSITION,0);
		m_pDeclaration->AddElement(0,12,DT_FLOAT3,DU_NORMAL,0);
		m_pDeclaration->AddElement(0,24,DT_FLOAT2,DU_TEXCOORD0,0);
		m_pDeclaration->AddElement(0,32,DT_FLOAT1,DU_TEXCOORD1,0);
		m_pDeclaration->Active();

// 		D3DVERTEXELEMENT9 VertexElements[] =
// 		{
// 			{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, 
// 			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
// 			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	
// 			{ 0, 32, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	
// 			D3DDECL_END()
// 		};
// 
// 		V( g_pD3DDevice->CreateVertexDeclaration(VertexElements, &m_pVertexDeclaration) );
	}

// 	static int GetMin( int a, int b )
// 	{
// 		return a<b?a:b;
// 	}
// 
// 	static int GetMax( int a, int b )
// 	{
// 		return a>b?a:b;
// 	}

	void TerrainLiquid::addGrid( int x, int z )
	{
		POINT point( x,z );
		m_pTempGridArray.push_back( point );
		//mBox.merge( __getPos( x,z ) );
		//mBox.merge( __getPos( x+1, z+1) );
	}

	void TerrainLiquid::createVertexData()
	{	
		m_pTempGridArray.sort();

		sLiquidVertex* pVerts = new sLiquidVertex[m_pTempGridArray.size() * 4];

		std::list<POINT>::iterator it;
		int i = 0;
		for( it = m_pTempGridArray.begin(); it != m_pTempGridArray.end(); it++, i += 4 )
		{
			POINT point = (*it);

			// left top
			pVerts[i].vPos = getPos( point.x, point.z );
			pVerts[i].vNormal = Vector3( 0.0f, 1.0f, 0.0f );
			pVerts[i].vVetrexUV0 = getTexCoord( point.x, point.z, LEFT_TOP );							
			if( m_bDepthEnable )											
			{																
				pVerts[i].vVetrexUV1  = getDepthTexCoord( point.x, point.z );						
			}																	

			// right top
			pVerts[i + 1].vPos = getPos( point.x + 1, point.z );
			pVerts[i + 1].vNormal = Vector3( 0.0f, 1.0f, 0.0f );
			pVerts[i + 1].vVetrexUV0 = getTexCoord( point.x + 1, point.z, RIGHT_TOP );											
			if( m_bDepthEnable )											
			{																	
				pVerts[i + 1].vVetrexUV1 = getDepthTexCoord( point.x + 1, point.z );						
			}																		

			// left bottom
			pVerts[i + 2].vPos = getPos( point.x, point.z + 1 );	
			pVerts[i + 2].vNormal = Vector3( 0.0f, 1.0f, 0.0f );
			pVerts[i + 2].vVetrexUV0 = getTexCoord( point.x, point.z + 1, LEFT_BOTTOM );												
			if( m_bDepthEnable )											
			{																	
				pVerts[i].vVetrexUV1 = getDepthTexCoord( point.x, point.z + 1 );								
			}																		

			// right bottom		
			pVerts[i + 3].vPos = getPos( point.x + 1, point.z + 1 );	
			pVerts[i + 3].vNormal = Vector3( 0.0f, 1.0f, 0.0f );
			pVerts[i + 3].vVetrexUV0 = getTexCoord( point.x + 1, point.z + 1, RIGHT_BOTTOM );											
			if( m_bDepthEnable )											
			{																
				pVerts[i].vVetrexUV1 = getDepthTexCoord( point.x + 1, point.z + 1 );									
			}		

// 			m_WorldAABB.Merge(pVerts[i].vPos);
// 			m_WorldAABB.Merge(pVerts[i + 1].vPos);	
// 			m_WorldAABB.Merge(pVerts[i + 2].vPos);	
// 			m_WorldAABB.Merge(pVerts[i + 3].vPos);	
		}

		UINT nVertSize = m_pTempGridArray.size() * 4 * sizeof(sLiquidVertex);
		m_pVertexBuffers = GetRenderDevice()->CreateVertexBuffer(pVerts,nVertSize);
		m_pVertexBuffers->Active();

		m_pSubMeshData->m_nVertexCount = m_pTempGridArray.size() * 4;

// 		HRESULT hr = 0;
// 		hr = g_pD3DDevice->CreateVertexBuffer( m_pTempGridArray.size() * 4 * sizeof(sLiquidVertex),
// 			D3DUSAGE_WRITEONLY,0,D3DPOOL_MANAGED,&m_pVertex,0);
// 		if( FAILED(hr) )
// 			return ;
// 
// 		sLiquidVertex* vBuf = 0;
// 		m_pVertex->Lock(0, 0, (void**)&vBuf, 0);
// 		memcpy( vBuf, pVerts, m_pTempGridArray.size() * 4 * sizeof(sLiquidVertex) );	
// 		m_pVertex->Unlock();
// 
// 		SAFE_DELETE_ARRAY(pVerts);
	}

	bool TerrainLiquid::isValidGrid( int x, int z, int dir )
	{
		int y = mSeedPos.y;

		int left =  m_pTerrainSystem->m_WorldOffset.x * -1;//(m_pTerrainSystem->m_heightMapW - 1)* m_pTerrainSystem->m_scale.x * -0.5;  //m_pTerrainSystem->m_worldExtents.x0;
		int right = m_pTerrainSystem->m_WorldOffset.x; //left + (m_pTerrainSystem->tableWidth() - 1) * m_pTerrainSystem->m_cellSize.x;
		int top =   m_pTerrainSystem->m_WorldOffset.z * -1;//(m_pTerrainSystem->m_heightMapH - 1) * m_pTerrainSystem->m_scale.z * -0.5;    //m_pTerrainSystem->m_worldExtents.y0;
		int bottom = m_pTerrainSystem->m_WorldOffset.z; //top + (m_pTerrainSystem->tableHeight() - 1) * m_pTerrainSystem->m_cellSize.y;

		Vector3 leftTop = getPos( x, z );
		Vector3 rightTop = getPos( x + 1, z );
		Vector3 leftBottom = getPos( x, z + 1);
		Vector3 rightBottom = getPos( x + 1, z + 1 );

		int lt = m_pTerrainSystem->GetHeight( leftTop.x, leftTop.z );
		int rt = m_pTerrainSystem->GetHeight( rightTop.x, rightTop.z );
		int lb = m_pTerrainSystem->GetHeight( leftBottom.x, leftBottom.z );
		int rb = m_pTerrainSystem->GetHeight( rightBottom.x, rightBottom.z );

		// bounding check
		if( leftTop.x < left || rightTop.x > right || leftTop.z < top || leftBottom.z > bottom )
			return false;

		if( lt > leftTop.y && rt > rightTop.y && lb > leftBottom.y && rb > rightBottom.y )
			return false;

		else if( dir == LEFT )
		{
			if( ( lt < y || lb < y ) && ( rt >=y && rb >=y) )
				return false;
		}

		else if( dir == RIGHT )
		{
			if( ( rt < y || rb < y ) && ( lt >= y && lb >= y ) )
				return false;
		}

		else if( dir == UP )
		{
			if( ( rt < y || lt < y ) && ( rb >= y && lb >= y ) )
				return false;
		}

		else if( dir == DOWN )
		{
			if( ( rb < y || lb < y ) && ( rt >=y && lt >= y ) )
				return false;
		}

		return true;
	}

	void TerrainLiquid::createIndexData()
	{
		int total_indexes = m_pTempGridArray.size() * 6;
		Uint16* pIndexValues = new Uint16[total_indexes];
		Uint16* pIdx = pIndexValues;
		Uint16 start_vert = 0;

		for( int i = 0; i < m_pTempGridArray.size(); ++i ) 
		{
			Uint16 iIndexTopLeft = i * 4;
			Uint16 iIndexTopRight = iIndexTopLeft + 1;
			Uint16 iIndexBottomLeft = iIndexTopLeft + 2;
			Uint16 iIndexBottomRight = iIndexTopLeft + 3;

			*pIdx++ = iIndexTopLeft;
			*pIdx++ = iIndexBottomLeft;
			*pIdx++ = iIndexTopRight;
			*pIdx++ = iIndexBottomLeft;
			*pIdx++ = iIndexBottomRight;
			*pIdx++ = iIndexTopRight;		
		}

		m_pIndexBuffer = GetRenderDevice()->CreateIndexBuffer(pIndexValues,total_indexes * sizeof(WORD));
		m_pIndexBuffer->Active();

		m_pSubMeshData->m_nIndexCount = total_indexes;
// 		HRESULT hr = D3D_OK;	
// 		hr = g_pD3DDevice->CreateIndexBuffer(total_indexes * sizeof(WORD), 
// 			D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pIndex,0);
// 		if( FAILED(hr) )
// 			return ;
// 
// 		WORD* indices = 0;
// 		m_pIndex->Lock(0, 0, (void**)&indices, 0);
// 		memcpy( indices, pIndexValues, total_indexes * sizeof(WORD) );	
// 		m_pIndex->Unlock();
// 
// 		delete [] pIndexValues;
	}


	Vector2 TerrainLiquid::getTexCoord( int x, int z, int type )
	{
		return Vector2( x * m_fTextureScale, z * m_fTextureScale );
	}

	float TerrainLiquid::getDepthTexCoord( int x, int z )
	{
		Vector3 pos = getPos(x,z);
		float terrainHieght = m_pTerrainSystem->GetHeight(pos.x, pos.z); 

		float depth = mSeedPos.y - terrainHieght;
		if( depth < 0.0f )
			depth = 0.0f;

		if( !m_bDepthEnable )
			return 1.0f;

		return depth * m_fDepthScale;
	}

	void TerrainLiquid::spreed( int x, int z, int direction )
	{
		if( !isGridContained( x, z ) && isValidGrid( x,z, direction ) ) 
			addGrid( x, z );
		else 
			return;

		spreed( x, z-1, UP );
		spreed( x, z+1 , DOWN );
		spreed( x-1, z, LEFT );
		spreed( x+1, z , RIGHT );
	}

	bool TerrainLiquid::isGridContained( int x, int z )
	{
		POINT p( x, z );
		if( find( m_pTempGridArray.begin(), m_pTempGridArray.end(), p ) == m_pTempGridArray.end() )
			return false;
		else 
			return true;
	}

	void TerrainLiquid::spreedOneStep( int x, int z )
	{
		if( !isGridContained( x, z ) )
			addGrid( x, z );
	}

	void TerrainLiquid::destroy()
	{
		//cSceneObject::destroy();
	}


	void TerrainLiquid::Render()
	{
// 		HRESULT hr = D3D_OK;
// 
// 		if( g_pD3DDevice )
// 		{
// 			g_pD3DDevice->SetIndices(m_pIndex);
// 			g_pD3DDevice->SetStreamSource( 0, m_pVertex, 0, sizeof(sLiquidVertex) );
// 
// 			V( g_pD3DDevice->SetFVF(NULL) );
// 			V( g_pD3DDevice->SetVertexDeclaration(m_pVertexDeclaration) );
// 
// 			const static float dunTime = 0.01f;  
// 			//float fDeltaTime = DeltaTimeUpdate(mfLastUpdate);
// 			double mfLastUpdate = GetAccurateTime();
// 			//mfLastUpdate += TheGameHost.elapsedApptime();
// 			// 		while (mfLastUpdate >= dunTime) 
// 			// 			mfLastUpdate -= dunTime;
// 			// 		while (mfLastUpdate < 0) 
// 			// 			mfLastUpdate += dunTime;
// 			int nToltalFrame = m_allFrameTex.size();
// 			int ncurFrame = 0;//( (int)(mfLastUpdate / dunTime * nToltalFrame) ) % nToltalFrame;
// 
// 			g_pCurCEffect->effect()->SetTexture( "frameTex", m_allFrameTex[ncurFrame] );
// 			g_pCurCEffect->effect()->SetTexture( "depthTex", m_pDepthTex );
// 
// 			// if set effect parm,must be use before every DrawIndexedPrimitive
// 			if ( g_pCurCEffect )
// 				g_pCurCEffect->CommitChanges(); 
// 
// 			hr =g_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
// 				0, 0, m_pTempGridArray.size() * 4, 0, m_pTempGridArray.size() * 2 );
// 
// 			assert( SUCCEEDED(hr) );
// 
// 			//g_pD3DDevice->SetTexture(0, NULL);
// 		}
	}


	// void TerrainLiquid::update()
	// {
	// 	if ( m_allFrameTex.empty() )
	// 		return;
	// 
	// 	const static float dunTime = 0.1f;  
	// 
	// 	mTime += TheGameHost.elapsedApptime();
	// 	while (mTime >= dunTime) 
	// 		mTime -= dunTime;
	// 	while (mTime < 0) 
	// 		mTime += dunTime;
	// 
	// 	int nFrame = m_allFrameTex.size();
	// 	int ncurFrame = ( (int)(mTime / dunTime * nFrame) ) % nFrame;
	// 	//m_pSurfaceMaterial->setTexture( 0, allFrameTex[ncurFrame] );
	// }

}






