#include "terrain.h"
#include "TerrainSection.h"
#include "effect_file.h"
#include "GameApp.h"
#include "OctreeSceneManager.h"
#include "tinyxml.h"

#include <fstream>
#include <cmath>
#include <algorithm>


CTerrain::CTerrain()
{
	m_pAltasTex = NULL;
	m_heightmap = NULL;
}

CTerrain::~CTerrain()
{
	SAFE_RELEASE(m_pAltasTex);
	SAFE_DELETE_ARRAY(m_heightmap);
}

void CTerrain::Create(std::string sTerrainName)
{	
	LoadTerrain(sTerrainName);

	m_WorldOffset.x = (m_heightMapW - 1) * m_scale.x * 0.5;
	m_WorldOffset.y = 0;
	m_WorldOffset.z = (m_heightMapH - 1) * m_scale.z * 0.5;

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

			int index = (y * m_sectorCountX) + x;
			
			CTerrainSection* pTerrainScetion = new CTerrainSection;
			pTerrainScetion->Create(this, heightMapX, heightMapY, m_sectorVerts, m_sectorVerts);
			g_SceneMng.AddTerrainScetion(pTerrainScetion);
		}
	}
}

void CTerrain::LoadTerrain(std::string sTerrainName)
{
	int nPos = sTerrainName.rfind('\\');
	std::string strDir = sTerrainName.substr(0,nPos+1);

	TiXmlDocument terrainDoc;
	bool bLoadSuccess = terrainDoc.LoadFile(sTerrainName);
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
	const char* gridInfoType = element->Attribute( "type");
	fileName = strDir + gridInfoFileName;
	LoadGridInfo( fileName.c_str() );

	// textures
	element = root->FirstChildElement( "textures");
	TiXmlElement* pTxtureElement = element->FirstChildElement();
	int imageIndex = 0;
	while( pTxtureElement )
	{
		elementValue = pTxtureElement->Attribute( "type");
		{	
			//if( cString("image") == elementValue )
			if ( strcmp(elementValue,"image") == 0 )
			{
				elementValue = pTxtureElement->Attribute("filename");
				elementValue = UTF8ToANSI( elementValue );
				std::string fileName = strDir + std::string("brush\\") + elementValue;
				m_vTextureFileNames.push_back( fileName.c_str() );
			}
			pTxtureElement = pTxtureElement->NextSiblingElement();
		}
	}
	m_vTextureFileNames.push_back(strDir + "transparent.tga");

	m_TextureAtlas.Create(g_pD3DDevice,m_vTextureFileNames);
	//m_pAltasTex = m_TextureAtlas.GetAtlasTexture();
	LoadTexture("TerainTextAltas0.dds");

	element = root->FirstChildElement("pixmaps");
	TiXmlElement* pPixmapElement = element->FirstChildElement();
	while( pPixmapElement )
	{
		Pixmap pixmap;
		unsigned int res;
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

void CTerrain::LoadHeightMap(const char* pFileName)
{
	FILE* pf = fopen(pFileName, "rb");
	assert(pf);
	if (pf == NULL)
		return;

	fseek( pf, 8, SEEK_SET );
	fread( &m_heightMapW, 4,1, pf );
	fread( &m_heightMapH, 4,1, pf );
	m_heightmap = new float[m_heightMapW * m_heightMapH];
	if (m_heightmap == NULL)
		return;
	fread( m_heightmap, sizeof(float) * m_heightMapW * m_heightMapH, 1, pf );
	fclose(pf);
}


void CTerrain::LoadGridInfo(const char* pFileName)
{
	FILE* pf = fopen( pFileName, "rb" );
	assert(pf);
	if (pf == NULL)
		return;

	DWORD nMagic;
	// 版本号
	DWORD nVersion;
	// 地表宽度(横向格子数)

	// 7字节版本
	bool bLargeVersion = false;

	fread( &nMagic, 4,1, pf );
	fread( &nVersion, 4, 1, pf );

	int size_x, size_y;
	fread( &size_x, 4,1, pf );
	fread( &size_y, 4,1, pf );

	m_GridInfoArray.reserve(size_x * size_y);

	if( nVersion >= 0x00100001 )
		fread( &bLargeVersion, sizeof( bool ), 1, pf );

	uint8 op1,op0;
	uint16 layer1 =0,layer0=0;
	uint8 tr;

	for(int i = 0 ; i < size_x * size_y; ++i)
	{
		if( !bLargeVersion )
			fread( &layer0, 1,1, pf );
		else 
			fread( &layer0, 2,1,pf );
		fread( &op0,1,1, pf );
		if( !bLargeVersion )
			fread( &layer1, 1,1,pf );
		else 
			fread( &layer1, 2, 1, pf );
		fread( &op1, 1,1,pf );
		fread( &tr, 1,1,pf );

		g_info g;
		g.layer0 = layer0-1;
		g.layer1 = layer1-1;
		g.op0 = op0;
		g.op1 = op1;
		g.tri = tr;
		if( tr != 0 )
		{
			int a = 0;
		}

		m_GridInfoArray.push_back( g );
	}
	fclose( pf );
}

// 由高度图坐标得到地形世界位置
void CTerrain::GetPos(int row, int col, D3DXVECTOR3& vWorldPos)
{
// 	if (row >= m_heightMapW)
// 		row = m_heightMapW - 1;
// 	if (col >= m_heightMapH)
// 		col = m_heightMapH - 1;

	vWorldPos.x = m_scale.x * row - m_WorldOffset.x;
	vWorldPos.y = m_scale.y * m_heightmap[col * m_heightMapW + row] - m_WorldOffset.y;
	vWorldPos.z = m_scale.z * col - m_WorldOffset.z;
}

void CTerrain::GetNormal(int row, int col, D3DXVECTOR3& vNormal)
{
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

	
	D3DXVECTOR3 pos;
	GetPos(row, col, pos);
	D3DXVECTOR3 corners[7];
	int count = 0;

#define DIR(i,j)	\
	{D3DXVECTOR3 TempPos;\
	GetPos(row + i, col + j, TempPos); \
	corners[count++] =  TempPos - pos;} 

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

	D3DXVECTOR3 sum(0, 0, 0);
	for (int i = 1; i < count; ++i)
	{
		D3DXVECTOR3 n;
		D3DXVec3Cross(&n, &corners[i-1], &corners[i]);
		assert(n.y > 0);
		D3DXVec3Normalize(&n, &n);
		sum += n;
	}
	D3DXVec3Normalize(&sum, &sum);

	vNormal = sum;
}


float CTerrain::GetHeightmapData(int row, int col)
{
	if ( row < 0 || row >= m_heightMapW ||
		 col < 0 || col >= m_heightMapH )
	{
		assert(false && "GetHeightmapData error");
		return 0;
	}

	return m_heightmap[row * m_heightMapW + col];
}

bool CTerrain::LoadTexture(std::string fileName)
{
	HRESULT hr = 0;

	hr = D3DXCreateTextureFromFileEx(
		g_pD3DDevice,
		fileName.c_str(),
		1024,
		512,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		NULL,
		NULL,
		&m_pAltasTex);

	if(FAILED(hr))
	{
		int res = GetLastError();
		//::MessageBox(0,"D3DXCreateTextureFromFile:Failed",0,0);
		return false;
	}

	return true;
}


float CTerrain::GetHeight(float x, float z)
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

	float A = GetHeightmapData(row,   col) * m_scale.y;
	float B = GetHeightmapData(row,   col+1) * m_scale.y;
	float C = GetHeightmapData(row+1, col) * m_scale.y;
	float D = GetHeightmapData(row+1, col+1) * m_scale.y;

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
		height = A + Lerp(0.0f, uy, dx) + Lerp(0.0f, vy, dz);
	}
	else // lower triangle DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B

		// Linearly interpolate on each vector.  The height is the vertex
		// height the vectors u and v originate from {D}, plus the heights
		// found by interpolating on each vector u and v.
		height = D + Lerp(0.0f, uy, 1.0f - dx) + Lerp(0.0f, vy, 1.0f - dz);
	}

	return height;
}
