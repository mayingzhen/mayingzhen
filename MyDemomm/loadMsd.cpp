#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <d3dx9.h>
//#include <d3dtypes.h>
#include <assert.h>
#include <algorithm>


#include "loadMsd.h"

#define MAGIC_STRING_VERSION  "VERSION_"
#define MAGIC_STRING_LENGTH	  8
#define MAGIC_STRING_SKIN	  "SKINDATA"
#define MAGIC_STRING_MATERIAL "MTLSDATA"
#define MAGIC_STRING_ACTION	  "ACTSDATA"
#define MAGIC_STRING_BONE	  "BONEDATA"

#define MSD_VERSION_OLD -1	// 版本-1	老插件导出的版本
#define MSD_VERSION_2	2	// 版本2	增加了dwFlag，扩展msd文件信息

#define ACT_VERSION_OLD -1	// 版本-1	老插件导出的版本
#define ACT_VERSION_0	0	// 版本0	原始版本
#define ACT_VERSION_1	1	// 版本1	原始版本
#define ACT_VERSION_2	2	// 版本2	增加了dwFlag，扩展act文件信息

#define SKL_VERSION_OLD -1	// 版本-1	老插件导出的版本
#define SKL_VERSION_0	0	// 版本0	原始版本
#define SKL_VERSION_1	1	// 版本1	原始版本
#define SKL_VERSION_2	2	// 版本2	增加了dwFlag，扩展skl文件信息


CChaSkinMesh chaSkinMesh;

int LoadToMyBuffer(const char* fileName,char** ppMyBuffer)
{
	int fh = _open( fileName, _O_BINARY | _O_RDONLY);
	if (fh == -1)
	{
		assert(false);
		return -1;
	}

	DWORD dwReadLen = _filelength(fh);
	if (dwReadLen == 0)
	{
		assert(false);
		return -1;
	}

	char* pBuffer = new char[dwReadLen];
	if (!pBuffer)
	{
		_close(fh);
		return -1;
	}

	if ( _read(fh,pBuffer,dwReadLen) <= 0 )
	{
		delete[] pBuffer;
		_close(fh);
		return -1;
	}
	_close(fh);

	*ppMyBuffer = pBuffer;
	return dwReadLen;
}


DWORD GetDWORD(char** pMyBuffer)
{
	DWORD dwTemp = 0;
	memcpy( (char*)&dwTemp, *pMyBuffer, sizeof(DWORD) );
	*pMyBuffer += sizeof(DWORD);
	return dwTemp;
}

BOOL GetBOOL(char** pMyBuffer)
{
	BOOL dwTemp;
	memcpy( (char*)&dwTemp, *pMyBuffer, sizeof(BOOL) );
	*pMyBuffer += sizeof(BOOL);
	return dwTemp;
}

std::string GetStr(char** pMyBuffer)
{
	DWORD dwLenth = GetDWORD(pMyBuffer);
	assert(dwLenth < MAX_PATH);
	char szTemp[MAX_PATH] = {0};
	memcpy( szTemp, *pMyBuffer, dwLenth );
	*pMyBuffer += dwLenth;
	return szTemp;
}

D3DMATERIAL9 GetD3DMATERIAL(char** pMyBuffer)
{
	D3DMATERIAL9 temp;
	memcpy( (char*)&temp, *pMyBuffer, sizeof(D3DMATERIAL9) );
	*pMyBuffer += sizeof(D3DMATERIAL9);	
	return temp;
}

D3DXVECTOR3 GetD3DXVECTOR3(char** pMyBuffer)
{
	D3DXVECTOR3 temp;
	memcpy( (char*)&temp, *pMyBuffer, sizeof(D3DXVECTOR3) );
	*pMyBuffer += sizeof(D3DXVECTOR3);	
	return temp;
}


D3DXMATRIX GetD3DXMATRIX(char** pMyBuffer)
{
	float fMat[16] = {0};
	memcpy( (char*)fMat, *pMyBuffer, sizeof(float) * 16 );
	*pMyBuffer += sizeof(float) * 16;
	return fMat;   	
}

DWORD GetMsdVersion(char** pMyBuffer) // 读文件头，获得版本号
{
	char szName[MAX_PATH] = {0};
	memcpy(szName,*pMyBuffer,MAGIC_STRING_LENGTH);
	*pMyBuffer += MAGIC_STRING_LENGTH;

	DWORD dwMsdVersion;
	if( !stricmp(szName,MAGIC_STRING_VERSION) ) 
	{
		dwMsdVersion = GetDWORD(pMyBuffer);

		char szName[MAX_PATH] = {0};
		memcpy(szName,*pMyBuffer,MAGIC_STRING_LENGTH);
		*pMyBuffer += MAGIC_STRING_LENGTH;

		if( stricmp(szName,MAGIC_STRING_SKIN) )
			return -1;
	}
	else
	{
		dwMsdVersion = MSD_VERSION_OLD;
		if( stricmp(szName,MAGIC_STRING_SKIN) )
			return -1;
	}

	return dwMsdVersion;
}

DWORD GetActVersion(char** pMyBuffer) // 读文件头，获得版本号
{
	char szName[MAX_PATH] = {0};
	memcpy(szName,*pMyBuffer,MAGIC_STRING_LENGTH);
	*pMyBuffer += MAGIC_STRING_LENGTH;

	DWORD dwMsdVersion;
	if( !stricmp(szName,MAGIC_STRING_VERSION) ) 
	{
		dwMsdVersion = GetDWORD(pMyBuffer);

		char szName[MAX_PATH] = {0};
		memcpy(szName,*pMyBuffer,MAGIC_STRING_LENGTH);
		*pMyBuffer += MAGIC_STRING_LENGTH;

		if( stricmp(szName,MAGIC_STRING_ACTION) )
			return -1;
	}
	else
	{
		dwMsdVersion = MSD_VERSION_OLD;
		if( stricmp(szName,MAGIC_STRING_ACTION) )
			return -1;
	}

	return dwMsdVersion;
}




DWORD GetSklVersion(char** pMyBuffer,DWORD& dwSklFlag) // 读文件头，获得版本号
{
	char szName[MAX_PATH] = {0};
	memcpy(szName,*pMyBuffer,MAGIC_STRING_LENGTH);
	*pMyBuffer += MAGIC_STRING_LENGTH;

	DWORD dwSklVersion;
	if( !stricmp(szName,MAGIC_STRING_VERSION) ) 
	{
		dwSklVersion = GetDWORD(pMyBuffer);

		if(dwSklVersion >= SKL_VERSION_2 && dwSklVersion != SKL_VERSION_OLD)
			 dwSklFlag = GetDWORD(pMyBuffer);

		char szName[MAX_PATH] = {0};
		memcpy(szName,*pMyBuffer,MAGIC_STRING_LENGTH);
		*pMyBuffer += MAGIC_STRING_LENGTH;

		if( stricmp(szName,MAGIC_STRING_BONE) )
			return -1;
	}
	else
	{
		dwSklVersion = MSD_VERSION_OLD;
		if( stricmp(szName,MAGIC_STRING_ACTION) )
			return -1;
	}

	return dwSklVersion;
}


int LoadMsdData(const char* fileName,std::vector<CSubMesh>& vListSubmesh)
{
	vListSubmesh.clear();

	char* pMyBuffer = NULL;
	int bufLen = LoadToMyBuffer(fileName,&pMyBuffer);
	
	DWORD dwMsdVersion = GetMsdVersion(&pMyBuffer);
	if (dwMsdVersion == -1)
		return -1;

 	DWORD dwNumSkin = GetDWORD(&pMyBuffer);
	for (int i = 0; i < dwNumSkin; ++i)
	{
		CSubMesh subMesh;

		subMesh.m_sSubMeshName = GetStr(&pMyBuffer);

		DWORD dwMSDFlag = 0;
		if (dwMsdVersion >= MSD_VERSION_2 && dwMsdVersion != MSD_VERSION_OLD)
			dwMSDFlag = GetDWORD(&pMyBuffer);

		subMesh.m_matRoot  = GetD3DXMATRIX(&pMyBuffer);    // 物体局部坐标原点相对于模型世界坐标原点的变换矩阵
		subMesh.m_matOffset = GetD3DXMATRIX(&pMyBuffer);   // 物体局部坐标原点相对于父物体坐标原点的变换矩阵

		DWORD dwTemp = GetDWORD(&pMyBuffer);

		/////m_MeshRef.Release();
		char szTemp[MAGIC_STRING_LENGTH + 4] = {0};
		memcpy( (char*)szTemp, pMyBuffer, MAGIC_STRING_LENGTH );
		pMyBuffer += MAGIC_STRING_LENGTH;

		SMeshDataBase sMesh;
		memcpy( (char*)&sMesh, pMyBuffer, sizeof(SMeshDataBase) );
		pMyBuffer += sizeof(SMeshDataBase);

		subMesh.m_sMesh = sMesh;
		
		subMesh.m_dwVerBufSize = GetDWORD(&pMyBuffer);
		char* pVertexData = NULL;		////////////////////////////////////////内存顶点缓冲指针
		if (subMesh.m_dwVerBufSize > 0)
		{
			pVertexData = new char[subMesh.m_dwVerBufSize];
			memcpy(pVertexData, pMyBuffer, sMesh.m_dwVertexStride * sMesh.m_dwVerticesNum);
			pMyBuffer += sMesh.m_dwVertexStride * sMesh.m_dwVerticesNum;
			subMesh.m_pVertexData = pVertexData;

			subMesh.m_pVertexDataTemp = new char[subMesh.m_dwVerBufSize];
			memcpy( subMesh.m_pVertexDataTemp, pVertexData, subMesh.m_dwVerBufSize );
		}

		subMesh.m_dwIdxBufSize = GetDWORD(&pMyBuffer);
		char* pIndexData = NULL;     //////////////////////////////////////////内存索引缓冲指针
		if (subMesh.m_dwIdxBufSize > 0)
		{
			pIndexData = new char[subMesh.m_dwIdxBufSize];
			memcpy(pIndexData, pMyBuffer, subMesh.m_dwIdxBufSize);
			pMyBuffer += subMesh.m_dwIdxBufSize;

			subMesh.m_pIndexData = pIndexData;
		}

		subMesh.m_dwBonesPerVer = GetDWORD(&pMyBuffer);  
		if (subMesh.m_dwBonesPerVer > 0) ///////////////////////////////////////影响顶点骨骼
		{
			DWORD* pdwVertexInflunece = new DWORD[sMesh.m_dwVerticesNum * subMesh.m_dwBonesPerVer];
			memcpy( (char*)pdwVertexInflunece, pMyBuffer, sizeof(DWORD) * sMesh.m_dwVerticesNum * subMesh.m_dwBonesPerVer );
			pMyBuffer += sizeof(DWORD) * sMesh.m_dwVerticesNum * subMesh.m_dwBonesPerVer;

			subMesh.m_pdwVertexInflunece = pdwVertexInflunece;

			float* pfVertexWeight = new float[sMesh.m_dwVerticesNum * subMesh.m_dwBonesPerVer];
			memcpy( (char*)pfVertexWeight, pMyBuffer, sizeof(float) * sMesh.m_dwVerticesNum * subMesh.m_dwBonesPerVer );
			pMyBuffer += sizeof(float) * sMesh.m_dwVerticesNum * subMesh.m_dwBonesPerVer;

			subMesh.m_pfVertexWeight = pfVertexWeight;
		}

		subMesh.m_dwVerRedundant = GetDWORD(&pMyBuffer);
		DWORD* pdwVertexRedundant = new DWORD[sMesh.m_dwVerticesNum];
		memset( pdwVertexRedundant, 255 , sizeof(DWORD) * sMesh.m_dwVerticesNum );

		subMesh.m_pdwVertexRedundant = pdwVertexRedundant;

		for(DWORD i = 0; i < subMesh.m_dwVerRedundant; ++i)
		{
			DWORD dwTem = GetDWORD(&pMyBuffer);
			(subMesh.m_pdwVertexRedundant)[dwTem] = GetDWORD(&pMyBuffer);
		}
		
		subMesh.m_dwMaterialNum = GetDWORD(&pMyBuffer);
		subMesh.m_vMateria.clear();
		for(DWORD i = 0; i < subMesh.m_dwMaterialNum; i++)
		{
			char szName[MAX_PATH] = {0};
			memcpy( szName, pMyBuffer, MAGIC_STRING_LENGTH );
			pMyBuffer += MAGIC_STRING_LENGTH;
			
			CMaterialData materiaData;

			DWORD dwID = GetDWORD(&pMyBuffer); 
			DWORD dwFlag = GetDWORD(&pMyBuffer);
			materiaData.m_material = GetD3DMATERIAL(&pMyBuffer);
			DWORD dwTexID = GetDWORD(&pMyBuffer);
			materiaData.m_strTextureFileName = GetStr(&pMyBuffer);

			subMesh.m_vMateria.push_back(materiaData);
		}
		
		DWORD *pdwPrimitveMatList = new DWORD[sMesh.m_dwPrimitiveCount];
		memcpy( (char*)pdwPrimitveMatList, pMyBuffer, sizeof(DWORD) * sMesh.m_dwPrimitiveCount );
		pMyBuffer += sizeof(DWORD) * sMesh.m_dwPrimitiveCount; 

		subMesh.m_pdwPrimitveMatList = pdwPrimitveMatList;

		DWORD dwMaterialNum = subMesh.m_dwMaterialNum; 
		if (dwMsdVersion != MSD_VERSION_OLD)
		{
			DWORD* pdwMaterialFaceCount = new DWORD[dwMaterialNum + 1];
			memset( pdwMaterialFaceCount, 0, sizeof(DWORD) * dwMaterialNum );
			memcpy( (char*)pdwMaterialFaceCount, pMyBuffer, sizeof(DWORD) * dwMaterialNum );
			pMyBuffer += sizeof(DWORD) * dwMaterialNum;

			subMesh.m_pdwMaterialFaceCount = pdwMaterialFaceCount;

			DWORD* pdwMaterialFaceOffset = new DWORD[dwMaterialNum + 1];
			memset( pdwMaterialFaceOffset, 0, sizeof(DWORD) * dwMaterialNum );
			memcpy( (char*)pdwMaterialFaceOffset, pMyBuffer, sizeof(DWORD) * dwMaterialNum );
			pMyBuffer += sizeof(DWORD) * dwMaterialNum;

			subMesh.m_pdwMaterialFaceOffset = pdwMaterialFaceOffset;
		}

		DWORD dwBoneNum = GetDWORD(&pMyBuffer);
		if(dwBoneNum)
		{
			subMesh.m_mBoneMatrix.clear();
			for(int iBone = 0; iBone < dwBoneNum; iBone++)
			{
				std::string szName = GetStr(&pMyBuffer);
				D3DXMATRIX matBone = GetD3DXMATRIX(&pMyBuffer);
				subMesh.m_mBoneMatrix[szName] = matBone;
				subMesh.m_vBone.push_back(szName);
				subMesh.m_vMatrix.push_back(matBone);
				//m_plistBoneName->insert(m_plistBoneName->end()
				//	,BoneNameItem(std::string(szName),matBone));
			}
		}

		vListSubmesh.push_back(subMesh);
	}

	return 1;
}


enum ENUM_ACT_FILE_FLAG
{
	ENUM_ACT_FILE_FLAG_MATRIX				= 1<<0,		// 是否包含骨骼的绝对钜阵
	ENUM_ACT_FILE_FLAG_QUAT					= 1<<1,		// 是否包含相对四元数
	ENUM_ACT_FILE_FLAG_FRAME_SORT_MODE		= 1<<2,		// 没此标志：骨骼1帧1、骨骼2帧1...骨骼1帧2、骨骼2帧2		
	// 有此标志：骨骼1帧1、骨骼1帧2...骨骼2帧1、骨骼2帧2
	ENUM_ACT_FILE_FLAG_ORIGIN_FRAME_TIME	= 1<<3,		// 是否包含Max导出时的原始帧时间
};


void LoadActData(const char* fileName,CAction& act)
{
	char* pMyBuffer = NULL;
	int bufLen = LoadToMyBuffer(fileName,&pMyBuffer);

	DWORD dwActVersion = GetActVersion(&pMyBuffer);
	if (dwActVersion == -1)
		return ;
	
	DWORD dwActNum = GetDWORD(&pMyBuffer);

	DWORD dwACTFlag = 0;
	if(dwActVersion >= ACT_VERSION_2 && dwActVersion != ACT_VERSION_OLD)
	{
		dwACTFlag = GetDWORD(&pMyBuffer);
	}

	char szTemp[MAX_PATH] = {0};
	memcpy(szTemp,pMyBuffer,MAGIC_STRING_LENGTH);
	pMyBuffer += MAGIC_STRING_LENGTH;
	//if( !stricmp(szTemp,MAGIC_STRING_ACTION) )
	//{
		//not 
	//}

	DWORD nSize = GetDWORD(&pMyBuffer);
	act.m_nActionID = GetDWORD(&pMyBuffer);
	act.m_nFrame = GetDWORD(&pMyBuffer);

	act.m_nBoneNum = GetDWORD(&pMyBuffer);
	assert(act.m_nBoneNum != 0);

	act.m_listBoneName.clear();
	for(DWORD i = 0; i < act.m_nBoneNum; i++)
	{
		act.m_listBoneName.push_back( GetStr(&pMyBuffer) );
	}
	
	float* pfFrameTime = new float[act.m_nFrame];
	memcpy( pfFrameTime, pMyBuffer, sizeof(float) * act.m_nFrame );
	pMyBuffer += sizeof(float) * act.m_nFrame;

	act.m_pfFrameTime = pfFrameTime;

	act.m_fTotalTime = act.m_pfFrameTime[act.m_nFrame - 1];
 
	float* pfOriginFrameTime = NULL; // Max里的原始速度
	if(dwACTFlag & ENUM_ACT_FILE_FLAG_ORIGIN_FRAME_TIME)
	{
		pfOriginFrameTime = new float[act.m_nFrame];
		memcpy( pfOriginFrameTime, pMyBuffer, sizeof(float) * act.m_nFrame );
		pMyBuffer += sizeof(float) * act.m_nFrame;

		act.m_pfOriginFrameTime = pfOriginFrameTime;
	}

	D3DXMATRIX* pMatrixList = NULL;
	if(dwACTFlag & ENUM_ACT_FILE_FLAG_MATRIX)
	{
		pMatrixList = new D3DXMATRIX[act.m_nBoneNum * act.m_nFrame];
		memcpy( pMatrixList, pMyBuffer, sizeof(D3DXMATRIX) * act.m_nBoneNum * act.m_nFrame );
		pMyBuffer += sizeof(D3DXMATRIX) * act.m_nBoneNum * act.m_nFrame;

		act.m_pMatrixList = pMatrixList;
	}

	//CountTotalTime();
	//fTotalTime = pfFrameTime[nFrame-1];
}

void LoadChidBoneData(char** pMyBuffer)
{
	std::string szChildBoneName = GetStr(pMyBuffer);

	DWORD dwTemp = GetDWORD(pMyBuffer);
 	for(DWORD i = 0; i < dwTemp; i++)
	{
		LoadChidBoneData(pMyBuffer);
	}
}


BoneMapByName g_allBone;    // 骨骼框架

void LoadSklData(const char* fileName,BoneMapByName& allBone)
{
	allBone.clear();

	char* pMyBuffer = NULL;
	int bufLen = LoadToMyBuffer(fileName,&pMyBuffer);

	DWORD dwSklFlage;
	DWORD dwSklVersion = GetSklVersion(&pMyBuffer,dwSklFlage);
	if (dwSklVersion == -1)
		return ;

	DWORD dwBoneNum = GetDWORD(&pMyBuffer);
	for( DWORD i = 0; i < dwBoneNum; i++)
	{
		std::string strBoneName = GetStr(&pMyBuffer);

		CBone bone;
		bone.bIsVisible = GetBOOL(&pMyBuffer);
		bone.vBoxMin = GetD3DXVECTOR3(&pMyBuffer);
		bone.vBoxMax = GetD3DXVECTOR3(&pMyBuffer);
		bone.matRoot = GetD3DXMATRIX(&pMyBuffer);
		bone.matOffset = GetD3DXMATRIX(&pMyBuffer);
		DWORD dwTemp = GetDWORD(&pMyBuffer);
		
		allBone[strBoneName] = bone;
		g_allBone[strBoneName] = bone;
	}

	LoadChidBoneData(&pMyBuffer);
}


void CChaSkinMesh::Build()
{
	LoadSklData("F:\\cha_player_01_nv_model.skl",m_allBone);

	std::vector<CSubMesh> vSubMesh;
 	LoadMsdData("F:\\cha_player_01_nan_head_01.msd",vSubMesh);
	m_chaMesh[0] =  vSubMesh;

	CAction act;
 	LoadActData("F:\\cha_player_01_nan_breath.act",act);
	m_nCurAct = act;
	
}

void CChaSkinMesh::Update(float fElapsedTime)
{
// 	if (fElapsedTime <= 0.01)
// 		return;

	DWORD nCurFrame = m_nCurAct.GetPositionByTime(fElapsedTime);

	m_nCurAct.Update(nCurFrame);

	std::vector<CSubMesh> subMesh = m_chaMesh[0];
	for(int i = 0; i < subMesh.size(); ++i)
		subMesh[i].Update(nCurFrame);
}


void CAction::Update(DWORD nCurFrame)
{
	DWORD dwOffset;
	for(DWORD dwCount=0; dwCount < m_nBoneNum; dwCount++)
	{
		dwOffset = nCurFrame * m_nBoneNum + dwCount;

		std::string boneName = m_listBoneName[dwCount];
		BoneMapByName::iterator it =  g_allBone.find(boneName);
		assert( it != g_allBone.end() );
		if ( it == g_allBone.end() )
			continue;
		
		CBone bone = it->second;
		bone.matRoot *= m_pMatrixList[dwOffset];
	}
}

DWORD CAction::GetPositionByTime(float& fRefTime)
{
	while( fRefTime > m_fTotalTime)
		fRefTime -= m_fTotalTime;

	DWORD i;
	for(i=0;i<m_nFrame;i++)
		if(m_pfFrameTime[i] >= fRefTime)
			return i;

	return 0;
}


void CSubMesh::Update(DWORD nCurFrame)
{
	D3DXMATRIX matLocalRoot[256];	
	memset( matLocalRoot, 0, sizeof(D3DXMATRIX) * 256 );
	std::map<std::string,D3DXMATRIX>::const_iterator it = m_mBoneMatrix.begin();
	for(int i = 0; i < m_vBone.size(); ++i)
	{
		D3DXMATRIX boneMatrix = m_vMatrix[i];
		std::string boneName = m_vBone[i];
		BoneMapByName::iterator it = g_allBone.find(boneName);
		assert( it != g_allBone.end() );
		D3DXMATRIX boneRootMatrix = (it->second).matRoot;
		matLocalRoot[i] = boneMatrix * boneRootMatrix;
	}

	float* fScrTemp=(float*)m_pVertexData;
	float* fTemp = (float*)m_pVertexDataTemp;
	float* fVec = NULL;
	float* pOutVec = NULL;
	DWORD i,j,nOffset(0);
	int nStride = m_sMesh.m_dwVertexStride >> 2;
	D3DXVECTOR3	vOut;
	for(int i = 0; i < m_sMesh.m_dwVerticesNum; i++)
	{
		if(-1 != m_pdwVertexRedundant[i])//跳过冗余顶点
			break;
		
		fVec=&fScrTemp[nOffset]; //position vector
		float* weight = &m_pfVertexWeight[m_dwBonesPerVer * i];
		DWORD* index = &m_pdwVertexInflunece[m_dwBonesPerVer * i];
		pOutVec= &fScrTemp[nOffset];
		ZeroMemory(pOutVec,12);

		for(int j= 0; j < m_dwBonesPerVer; j++)
		{
			if (weight[j] <= 0.0001f) // 权重过小则忽略
				continue;

			if(index[j] != -1 && index[j] < 256)
				D3DXVec3TransformCoord( &vOut, (D3DXVECTOR3*)fVec, &matLocalRoot[ index[j] ] );
			else
				vOut=fVec;

			pOutVec[0] += vOut.x*weight[j];
			pOutVec[1] += vOut.y*weight[j];
			pOutVec[2] += vOut.z*weight[j];
		}

		nOffset += nStride;// (3*vec+3*normal+2*tec)
	}

	//通过复制，更新冗余顶点的数据
	int nCnt = m_sMesh.m_dwVerticesNum - 3;
	for(int p = 0; p < nCnt; p+=4)
	{
		int nSp = p;
		if(-1 != m_pdwVertexRedundant[nSp])
		{
			j = m_pdwVertexRedundant[nSp];
			memcpy(fTemp+nStride*nSp,fTemp+nStride*j,12);
		}
		if(-1 != m_pdwVertexRedundant[++nSp])
		{
			j=m_pdwVertexRedundant[nSp];
			memcpy(fTemp+nStride*nSp,fTemp+nStride*j,12);
		}
		if(-1 != m_pdwVertexRedundant[++nSp])
		{
			j=m_pdwVertexRedundant[nSp];
			memcpy(fTemp+nStride*nSp,fTemp+nStride*j,12);
		}
		if(-1 != m_pdwVertexRedundant[++nSp])
		{
			j=m_pdwVertexRedundant[nSp];
			memcpy(fTemp+nStride*nSp,fTemp+nStride*j,12);
		}
	}

	for(i = (m_sMesh.m_dwVerticesNum&~3); i < m_sMesh.m_dwVerticesNum; i++)
	{
		if(-1 != m_pdwVertexRedundant[i])
		{
			j=m_pdwVertexRedundant[i];
			memcpy(fTemp+nStride*i,fTemp+nStride*j,12);
		}
	}

	//if( pVtxBuf != NULL )
	//	UpdateVertexBuffer(pVtxBuf, (LPVOID)fTemp);
	//memcpy(pVtxBuf,(LPVOID)fTemp,m_dwVerBufSize);
}

void CSubMesh::Render()
{
	//g_pD3DDevice->SetIndices(m_pIndexData)

}

void CChaSkinMesh::Render()
{

}


// void main()
// {
// 	chaSkinMesh.Build();
// 
// 	while (1)
// 	{
// 		static DWORD dwLastSaveTime = ::GetTickCount();
// 		DWORD dwCurTime = ::GetTickCount();
// 
// 		chaSkinMesh.Update(dwCurTime - dwLastSaveTime);
// 		chaSkinMesh.Render();
// 		dwLastSaveTime = dwCurTime;
// 		
// 		int t = 0;
// 		if (t)
// 			break;
// 	}
// }	
