#include "Terrain.h"
#include "TerrainSection.h"


#if PLATFORM_WIN == 1
char * UnicodeToANSI( const wchar_t* str )
{
	char* result;
	int textlen;
	textlen = WideCharToMultiByte( CP_ACP, 0, str, -1, NULL, 0, NULL, NULL );
	result =(char *)malloc((textlen+1)*sizeof(char));
	memset( result, 0, sizeof(char) * ( textlen + 1 ) );
	WideCharToMultiByte( CP_ACP, 0, str, -1, result, textlen, NULL, NULL );
	return result;
}


wchar_t * UTF8ToUnicode( const char* str )
{
	int textlen ;
	wchar_t * result;
	textlen = MultiByteToWideChar( CP_UTF8, 0, str,-1, NULL,0 );  
	result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t));  
	memset(result,0,(textlen+1)*sizeof(wchar_t));  
	MultiByteToWideChar(CP_UTF8, 0,str,-1,(LPWSTR)result,textlen );  
	return result;  
}

char* UTF8ToANSI(const char* str)
{	
	wchar_t* temp = UTF8ToUnicode(str);
	char* res = UnicodeToANSI(temp);
	delete []temp;
	return res;
}
#elif PLATFORM_ANDROID == 1 || PLAFTORM_IOS == 1
char* UTF8ToANSI(const char* str)
{
	return (char*)str;
}
#endif


namespace ma
{
	Terrain::Terrain()
	{
		m_pAltasTex = NULL;
		m_heightmap = NULL;
	}

	Terrain::~Terrain()
	{
		//SAFE_RELEASE(m_pAltasTex);
		SAFE_DELETE_ARRAY(m_heightmap);

// 		for (UINT i = 0; i < m_arrSection.size(); ++i)
// 		{	
// 			SAFE_DELETE(m_arrSection[i]);
// 		}
	}

	void Terrain::Update()
	{
// 		for (UINT i = 0; i < m_arrSection.size(); ++i)
// 		{	
// 			GetRenderQueue()->AddRenderObj(RL_Solid,m_arrSection[i]);
// 		}
	}

	void Terrain::Create(const char* pPath,Scene* pScene)
	{	
		ASSERT(pScene);
		if (pScene == NULL)
			return;

		LoadTerrain(pPath);

		m_WorldOffset.x = (m_heightMapW - 1) * m_scale.x * 0.5f;
		m_WorldOffset.y = (m_heightMapH - 1) * m_scale.y * 0.5f;
		m_WorldOffset.z = 0;

		int shift = 5;
		m_sectorShift = shift;
		m_sectorUnits = 1 << shift;
		m_sectorVerts = m_sectorUnits + 1; 

		m_sectorCountX = m_heightMapW >> m_sectorShift;
		m_sectorCountY = m_heightMapH >> m_sectorShift;

		for (int y = 0; y < m_sectorCountY; ++y)
		{
			for (int x = 0; x < m_sectorCountX; ++x)
			{
				int heightMapX = x << m_sectorShift;
				int heightMapY = y << m_sectorShift;

				SceneNodePtr pGameObj = pScene->CreateNode("TerrainSection");
				
				TerrainSection* pTerrainScetion =  new TerrainSection(this);
				pGameObj->AddComponent(pTerrainScetion);
				pTerrainScetion->Create(heightMapX, heightMapY, m_sectorVerts, m_sectorVerts);
				m_arrSection.push_back(pTerrainScetion);

				pScene->GetCullTree()->AddObject(pTerrainScetion);
			}
		}
	}

	void Terrain::LoadTerrain(const char* pPath)
	{
		std::string sTerrainName = GetArchiveMananger()->GetFullPath(pPath);
		std::string strDir = StringUtil::getDirectoryName(pPath) + "/";

		TiXmlDocument terrainDoc;
		bool bLoadSuccess = terrainDoc.LoadFile(sTerrainName.c_str());
		if( !bLoadSuccess ) 
		{
			assert(false && "load terrain file failed");
			return;
		}

		TiXmlElement* root = terrainDoc.FirstChildElement(); 
		TiXmlElement* element  = NULL;

		const char* elementValue;

		// sacle
		element = root->FirstChildElement("scale");
		m_scale.x = (float)atoi( element->Attribute("x") );
		m_scale.y = (float)atoi( element->Attribute("y") );
		m_scale.z = (float)atoi( element->Attribute("z") );

		// heightmap
		element = root->FirstChildElement("heightmap");
		const char* heightMapName = element->Attribute("filename");
		std::string fileName = strDir + heightMapName;
		LoadHeightMap( fileName.c_str() );

		// gridinfo
		element = root->FirstChildElement( "gridInfo" );
		const char* gridInfoFileName = element->Attribute( "filename" );
		fileName = strDir + gridInfoFileName;
		LoadGridInfo( fileName.c_str() );

		// textures
		element = root->FirstChildElement( "textures");
		TiXmlElement* pTxtureElement = element->FirstChildElement();
		while( pTxtureElement )
		{
			elementValue = pTxtureElement->Attribute( "type");
			{	
				if ( strcmp(elementValue,"image") == 0 )
				{
					elementValue = pTxtureElement->Attribute("filename");
					elementValue = UTF8ToANSI( elementValue );
					//std::string fileName = strDir + std::string("brush/") + elementValue;
					m_vTextureFileNames.push_back( elementValue );
				}
				pTxtureElement = pTxtureElement->NextSiblingElement();
			}
		}
		m_vTextureFileNames.push_back(strDir + "transparent.tga");

		std::string altasPath = strDir + "/teraintextaltas.tai";
		LoadTextAltas(altasPath.c_str());

		element = root->FirstChildElement("pixmaps");
		TiXmlElement* pPixmapElement = element->FirstChildElement();
		while( pPixmapElement )
		{
			PixMap pixmap;
			UINT res;
			res = pPixmapElement->QueryFloatAttribute( "top", &pixmap.top );
			if(TIXML_NO_ATTRIBUTE == res )
				pixmap.top = 0;
			res = pPixmapElement->QueryFloatAttribute( "bottom", &pixmap.bottom );
			if(TIXML_NO_ATTRIBUTE == res )
				pixmap.bottom = 1.0f;
			res = pPixmapElement->QueryFloatAttribute( "left", &pixmap.left);
			if( TIXML_NO_ATTRIBUTE == res )
				pixmap.left = 0.0f;
			res = pPixmapElement->QueryFloatAttribute( "right", &pixmap.right );
			if( TIXML_NO_ATTRIBUTE == res )
				pixmap.right = 1.0f;

			res = pPixmapElement->QueryIntAttribute( "textureId", &pixmap.textureId );

			m_PixMapArray.push_back(pixmap);

			pPixmapElement = pPixmapElement->NextSiblingElement();
		}
	}

	void Terrain::LoadHeightMap(const char* pFileName)
	{
		StreamPtr pStream = GetArchiveMananger()->Open(pFileName);
		if (pStream == NULL)
			return;

		pStream->Seek(8);
		pStream->Read(&m_heightMapW,4);
		pStream->Read(&m_heightMapH,4);
		m_heightmap = new float[m_heightMapW * m_heightMapH];
		pStream->Read( m_heightmap, 4 * m_heightMapW * m_heightMapH);
		pStream->Close();
	}


	void Terrain::LoadGridInfo(const char* pFileName)
	{
		StreamPtr pStream = GetArchiveMananger()->Open(pFileName);
		if (pStream == NULL)
			return;

		DWORD nMagic;
		// 版本号
		DWORD nVersion;
		// 地表宽度(横向格子数)

		// 7字节版本
		bool bLargeVersion = false;

		pStream->Read(&nMagic, 4);
		pStream->Read(&nVersion, 4);

		int size_x, size_y;
		pStream->Read(&size_x, 4);
		pStream->Read(&size_y, 4);

		m_GridInfoArray.reserve(size_x * size_y);

		if( nVersion >= 0x00100001 )
			pStream->Read( &bLargeVersion, 1);

		Uint8 op1,op0;
		Uint16 layer1 =0,layer0=0;
		Uint8 tr;

		for(int i = 0 ; i < size_x * size_y; ++i)
		{
			if( !bLargeVersion )
				pStream->Read(&layer0, 1);
			else 
				pStream->Read(&layer0, 2);
			pStream->Read(&op0,1);
			if( !bLargeVersion )
				pStream->Read(&layer1, 1);
			else 
				pStream->Read(&layer1, 2);
			pStream->Read(&op1, 1);
			pStream->Read(&tr, 1);

			GridInfo g;
			g.layer0 = layer0-1;
			g.layer1 = layer1-1;
			g.op0 = op0;
			g.op1 = op1;
			g.tri = tr;
			if( tr != 0 )
			{
			}

			m_GridInfoArray.push_back( g );
		}
		pStream->Close();
	}

	// 由高度图坐标得到地形世界位置
	Vector3 Terrain::GetPos(int row, int col)
	{
		Vector3 vWorldPos;

		vWorldPos.x = m_scale.x * row - m_WorldOffset.x;
		vWorldPos.y = m_scale.y * col - m_WorldOffset.y;
		vWorldPos.z = m_scale.y * m_heightmap[col * m_heightMapW + row] - m_WorldOffset.y;
		
		return  vWorldPos;
	}

	Vector3 Terrain::GetNormal(int row, int col)
	{
		return Vector3::UNIT_Z;
		/*
			   x-1 x x+1
			z-1 +--+--+
				| /| /|
				|/ |/ |
			z   +--+--+
				| /| /|
				|/ |/ |
			z+1 +--+--+
		*/

		Vector3 vNormal;
		
		Vector3 pos = GetPos(row, col);
		Vector3 corners[7];
		int count = 0;

		#define DIR(i,j) {Vector3 TempPos = GetPos(row + i, col + j); corners[count++] =  TempPos - pos;} 

		if (row == 0)
		{
			if (col != m_heightMapH - 1)
			{
				DIR( 0,+1);
			}
			DIR(+1, 0);
			if (col != 0)
			{
				DIR(+1,-1);
				DIR( 0,-1);
			}
		}
		else if (row == m_heightMapW - 1)
		{
			if (col != 0)
			{
				DIR( 0,-1);
			}
			DIR(-1, 0);
			if (col != m_heightMapH - 1)
			{
				DIR(-1,+1);
				DIR( 0,+1);
			}
		}
		else
		{
			if (col != 0)
			{
				DIR(+1, 0);
				DIR(+1,-1);
				DIR(0 ,-1);
			}
			DIR(-1, 0);
			if (col != m_heightMapH - 1)
			{
				DIR(-1,+1);
				DIR( 0,+1);
				DIR(+1, 0);
			}
		}
	#undef DIR

		assert(2 <= count && count <= sizeof(corners)/sizeof(*corners));

		Vector3 sum(0, 0, 0);
		for (int i = 1; i < count; ++i)
		{
			Vector3 n = corners[i-1].crossProduct(corners[i]);
			assert(n.y > 0);
			n.normalise();
			sum += n;
		}
		sum.normalise();

		vNormal = sum;

		return vNormal;
	}


	float Terrain::GetHeightmapData(int row, int col)
	{
		if ( row < 0 || row >= m_heightMapW ||
			 col < 0 || col >= m_heightMapH )
		{
			assert(false && "GetHeightmapData error");
			return 0;
		}

		return m_heightmap[row * m_heightMapW + col];
	}

	void Terrain::LoadTextAltas(const char* psAltaPath)
	{
		std::string sFilPath = GetArchiveMananger()->GetFullPath(psAltaPath);

		TiXmlDocument alta;
		bool bLoadSuccess = alta.LoadFile(sFilPath.c_str());
		ASSERT(bLoadSuccess && "load terrain file failed");
		if (!bLoadSuccess)
			return;
		
		TiXmlElement* pELeTextAltas = alta.FirstChildElement("TextAltas"); 
		ASSERT(pELeTextAltas);
		if (pELeTextAltas == NULL)
			return;

		std::string outBasename, outPath;
		StringUtil::splitFilename(psAltaPath,outBasename,outPath);

		const char* pTextAltasPath = pELeTextAltas->Attribute("path");
		std::string sTextAltasPath = outPath + pTextAltasPath;
		m_pAltasTex = LoadResourceASync<Texture>(sTextAltasPath.c_str(),NULL);

		SamplerStatePtr pSameler = new SamplerState();
		pSameler->SetWrapMode(CLAMP);
		pSameler->SetFilterMode(TFO_TRILINEAR);
		pSameler->SetTexture(m_pAltasTex);
		m_pMaterial = new Material();
		Technique* pTechnique = m_pMaterial->LoadTechnique("terrain","");
		pTechnique->SetParameter("TerrainTex",Any(pSameler));
		
		TiXmlElement* pEleTexture = pELeTextAltas->FirstChildElement("Texture");
		while (pEleTexture)
		{
			TextureInAlta inAlta;

			const char* pFileName =  pEleTexture->Attribute("fileName");
			const char* puOffset = pEleTexture->Attribute("uOffset");
			const char* pvOffset = pEleTexture->Attribute("vOffset");
			const char* puWidth = pEleTexture->Attribute("uWidth");
			const char* pvHeight = pEleTexture->Attribute("vHeight");
			
			pFileName = UTF8ToANSI(pFileName);

			inAlta.mpFilename = pFileName ? pFileName : "";
			inAlta.uOffset = puOffset ? (float)atof(puOffset) : 0;
			inAlta.vOffset = pvOffset ? (float)atof(pvOffset) : 0;
			inAlta.uWidth = puWidth ? (float)atof(puWidth) : 0;
			inAlta.vHeight = pvHeight ? (float)atof(pvHeight) : 0;

			m_TextureAtlas.push_back(inAlta);

			pEleTexture = pEleTexture->NextSiblingElement();
		}
		
	}


	float Terrain::GetHeight(float x, float z)
	{
		x += m_WorldOffset.x;
		z += m_WorldOffset.z;
		x /= (float)m_scale.x;
		z /= (float)m_scale.z;

		// From now on, we will interpret our positive z-axis as
		// going in the 'down' direction, rather than the 'up' direction.
		// This allows to extract the row and column simply by 'flooring'
		// x and z:
		float col = ::floorf(x);
		float row = ::floorf(z);

		// get the heights of the quad we're in:
		// 
		//  A   B
		//  *---*
		//  | / |
		//  *---*  
		//  C   D

		float A = GetHeightmapData((int)row,   (int)col) * m_scale.y;
		float B = GetHeightmapData((int)row,   (int)col+1) * m_scale.y;
		float C = GetHeightmapData((int)row+1, (int)col) * m_scale.y;
		float D = GetHeightmapData((int)row+1, (int)col+1) * m_scale.y;

		//
		// Find the triangle we are in:
		//

		// Translate by the transformation that takes the upper-left
		// corner of the cell we are in to the origin.  Recall that our 
		// cellspacing was nomalized to 1.  Thus we have a unit square
		// at the origin of our +x -> 'right' and +z -> 'down' system.
		float dx = x - col;
		float dz = z - row;

		// Note the below compuations of u and v are unneccessary, we really
		// only need the height, but we compute the entire vector to emphasis
		// the books discussion.
		float height = 0.0f;
		if(dz < 1.0f - dx)  // upper triangle ABC
		{
			float uy = B - A; // A->B
			float vy = C - A; // A->C

			// Linearly interpolate on each vector.  The height is the vertex
			// height the vectors u and v originate from {A}, plus the heights
			// found by interpolating on each vector u and v.
			height = A + Math::Lerp(0.0f, uy, dx) + Math::Lerp(0.0f, vy, dz);
		}
		else // lower triangle DCB
		{
			float uy = C - D; // D->C
			float vy = B - D; // D->B

			// Linearly interpolate on each vector.  The height is the vertex
			// height the vectors u and v originate from {D}, plus the heights
			// found by interpolating on each vector u and v.
			height = D + Math::Lerp(0.0f, uy, 1.0f - dx) + Math::Lerp(0.0f, vy, 1.0f - dz);
		}

		return height;
	}

	bool Terrain::GetInTextureAtlasUV(int textureID,float& texU, float& texV,float& texW, float& texH)
	{
		for (UINT i = 0; i < m_TextureAtlas.size(); ++i)
		{
			if (m_vTextureFileNames[textureID] ==  m_TextureAtlas[i].mpFilename) 
			{
				texU = m_TextureAtlas[i].uOffset;
				texV = m_TextureAtlas[i].vOffset;
				texW = m_TextureAtlas[i].uWidth;
				texH = m_TextureAtlas[i].vHeight;
				return true;
			}
		}
		ASSERT(false);
		return false;
	}

}

