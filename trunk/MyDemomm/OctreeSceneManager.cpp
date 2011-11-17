#include "public.h"
#include "OctreeSceneManager.h"
#include "CObject.h"
#include "OctreeNode.h"
#include "TextureMngr.h"
#include "ResMeshMngr.h"
#include "ResLoader.h"
#include "GameApp.h"

#include "tinyxml.h"

#include "Sky.h"
#include "Terrain.h"
#include "TerrainSection.h"
#include "TerrainLiquid.h"
#include "mouse.h"
#include "HDRPostProcess.h"

#include <math.h>



OctreeSceneManager g_SceneMng;

OctreeSceneManager::OctreeSceneManager()
{
    mOctree = 0;
}

void OctreeSceneManager::Init( CAABB &box, int depth )
{
    if ( mOctree != 0 )
        delete mOctree;

    mOctree = new COctreeNode(0);

    mMaxDepth = depth;
    mBox = box;

    mOctree -> mBox = box;

    D3DXVECTOR3 min = box.vMin;

    D3DXVECTOR3 max = box.vMax;

    mOctree -> mHalfSize = (max - min) / 2;

	m_pSelfCha = NULL;

	m_pRenderQueue = new CRenderQueue;

	InitLight();

}

OctreeSceneManager::~OctreeSceneManager()
{
    SAFE_DELETE(mOctree);
	SAFE_DELETE(m_pRenderQueue);

	for (int i = 0; i < m_pTerrainLiquids.size(); ++i)
	{
		SAFE_DELETE(m_pTerrainLiquids[i]);
	}
}

void OctreeSceneManager::ReleaseAll()
{
	std::list<CObject*>::iterator it; 
	for (it = m_allObject.begin(); it != m_allObject.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	TextureMngr()->Shutdown();
	ResMeshMngr()->Shutdown();

 	ResLoader()->Shutdown();
}

const std::list<CObject*>& OctreeSceneManager::GetRenderObjList()
{
	return m_allObject;
	//return m_pRenderQueue->m_NormalRQ;
	//return std::vector<CObject*>( m_allObject.begin(),m_allObject.end() );
}


void OctreeSceneManager::UpdateObject(CObject* object)
{
	assert(object);
	if (object == NULL)
		return;

    const CAABB& box = object->GetWorldAABB();

    if ( box.isNull() )
        return ;

	if (!mOctree)
		return;

    if (object->GetOctant() == 0)
    {
        if ( !object->IsIn( mOctree->mBox ) )  //if outside the octree, force into the root node.
            mOctree->AddObject(object);
        else
            AddObject(object,mOctree);
        return;
    }

    if ( !object->IsIn( object->GetOctant()->mBox ) )
    {
        RemoveObject(object);

        if ( !object->IsIn( mOctree->mBox ) )  //if outside the octree, force into the root node.
            mOctree->AddObject(object);
        else
            AddObject(object,mOctree);
    }
}


void OctreeSceneManager::RemoveObject(CObject* object)
{
	if ( !mOctree )
		return;

    COctreeNode* oct = object->GetOctant();

    if (oct)
        oct->RemoveObject(object);

    object->SetOctant(0);
}


void OctreeSceneManager::AddObject(CObject* object, COctreeNode* octant, int depth)
{
	if ( !mOctree )
		return;

	const CAABB& bx = object->GetWorldAABB();

    //if the octree is twice as big as the scene node,
    //we will add it to a child.
    if ( depth < mMaxDepth && octant->IsTwiceSize(bx) )
    {
        int x = 0, y = 0, z = 0;
        octant->GetChildIndexes(bx,&x,&y,&z);

        if ( octant->m_pChildren[x][y][z] == 0 )
        {
            octant -> m_pChildren[x][y][z] = new COctreeNode(octant);
            const D3DXVECTOR3& octantMin = octant -> mBox.vMin;
            const D3DXVECTOR3& octantMax = octant -> mBox.vMax;
            D3DXVECTOR3 min, max;

            if (x == 0)
            {
                min.x = octantMin.x;
                max.x = (octantMin.x + octantMax.x) / 2;
            }
            else
            {
                min.x = (octantMin.x + octantMax.x) / 2;
                max.x = octantMax.x;
            }

            if (y == 0)
            {
                min.y = octantMin.y;
                max.y = (octantMin.y + octantMax.y ) / 2;
            }
            else
            {
                min.y = (octantMin.y + octantMax.y) / 2;
                max.y = octantMax.y;
            }

            if (z == 0)
            {
                min.z = octantMin.z;
                max.z = (octantMin.z + octantMax.z) / 2;
            }
            else
            {
                min.z = (octantMin.z + octantMax.z) / 2;
                max.z = octantMax.z;
            }

            octant -> m_pChildren[x][y][z] -> mBox.setExtents( min, max );
            octant -> m_pChildren[x][y][z] -> mHalfSize = ( max - min ) / 2;
        }

        AddObject(object,octant->m_pChildren[x][y][z],++depth);
    }
    else
    {
        octant->AddObject(object);
    }
}


void OctreeSceneManager::FindVisibleObjects(const CFrustum& frustum)
{
	m_pRenderQueue->clear();

	walkOctree(frustum,mOctree,CFrustum::Visble::PARTIAL);
}



void OctreeSceneManager::Render()
{
	m_pRenderQueue->Render();
}


// Helper function for AABB vs frustum test
//
//
inline bool ProjectedIntersection(const D3DXVECTOR3 &vHalfSize, const D3DXVECTOR3 &vCenter,
								  const D3DXVECTOR3 *pFrustumPoints,
								  const D3DXVECTOR3 &vDir)
{
	// project AABB center point to vector
	float fCenter = D3DXVec3Dot(&vCenter, &vDir);
	// project AABB half-size to vector
	float fHalfSize = vHalfSize.x * fabs(vDir.x) +
		vHalfSize.y * fabs(vDir.y) +
		vHalfSize.z * fabs(vDir.z);

	float fMin1 = fCenter - fHalfSize;
	float fMax1 = fCenter + fHalfSize;

	// project frustum points
	float fProj2 = D3DXVec3Dot(&pFrustumPoints[0], &vDir);
	float fMin2 = fProj2;
	float fMax2 = fProj2;
	for(int i=1;i<8;i++)
	{
		fProj2 = D3DXVec3Dot(&pFrustumPoints[i], &vDir);
		fMin2 = Min(fProj2, fMin2);
		fMax2 = Max(fProj2, fMax2);
	}

	// test for overlap
	if(fMin1 > fMax2 || fMin2 > fMax1) return false;

	return true;
}

// AABB vs Frustum test, returns true if objects intersect
//
//
inline bool IntersectionTest(const CAABB& objectBB, const CFrustum &frustum)
{
	// Note that this code is very unoptimal
	//
	//
	D3DXVECTOR3 vHalfSize = (objectBB.vMax - objectBB.vMin) * 0.5f;
	D3DXVECTOR3 vCenter = (objectBB.vMin + objectBB.vMax) * 0.5f;

	// AABB face normals
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, D3DXVECTOR3(1,0,0))) return false;
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, D3DXVECTOR3(0,1,0))) return false;
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, D3DXVECTOR3(0,0,1))) return false;

	// frustum face normals
	//

	// front and back faces:
	D3DXVECTOR3 vNorm1;
	//= Normalize(Cross(frustum.vPts[1] - frustum.vPts[0],
	//	frustum.vPts[3] - frustum.vPts[0]));
	D3DXVec3Cross( &vNorm1, &(frustum.vPts[1] - frustum.vPts[0]), &(frustum.vPts[3] - frustum.vPts[0]) );
	D3DXVec3Normalize( &vNorm1, &vNorm1 );

	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm1)) return false;

	// left face:
	D3DXVECTOR3 vNorm2;
	//= Normalize(Cross(frustum.vPts[1] - frustum.vPts[0],
	//	frustum.vPts[4] - frustum.vPts[0]));
	D3DXVec3Cross( &vNorm2, &(frustum.vPts[1] - frustum.vPts[0]), &(frustum.vPts[4] - frustum.vPts[0]) );
	D3DXVec3Normalize( &vNorm2, &vNorm2 );
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm2)) return false;

	// right face:
	D3DXVECTOR3 vNorm3;
	//= Normalize(Cross(frustum.vPts[2] - frustum.vPts[3],
	//	frustum.vPts[7] - frustum.vPts[3]));
	D3DXVec3Cross( &vNorm3, &(frustum.vPts[2] - frustum.vPts[3]), &(frustum.vPts[7] - frustum.vPts[3]) );
	D3DXVec3Normalize( &vNorm3, &vNorm3 );
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm3)) return false;

	// top face:
	D3DXVECTOR3 vNorm4;
	//= Normalize(Cross(frustum.vPts[2] - frustum.vPts[1],
	//	frustum.vPts[5] - frustum.vPts[1]));
	D3DXVec3Cross( &vNorm4, &(frustum.vPts[2] - frustum.vPts[1]), &(frustum.vPts[5] - frustum.vPts[1]) );
	D3DXVec3Normalize( &vNorm4, &vNorm4 );
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm4)) return false;

	// bottom face:
	D3DXVECTOR3 vNorm5;
	//= Normalize(Cross(frustum.vPts[3] - frustum.vPts[0],
	//	frustum.vPts[4] - frustum.vPts[0]));
	D3DXVec3Cross( &vNorm5, &(frustum.vPts[3] - frustum.vPts[0]), &(frustum.vPts[4] - frustum.vPts[0]) );
	D3DXVec3Normalize( &vNorm5, &vNorm5 );
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm5)) return false;


	// edge cross edge cases
	//
	D3DXVECTOR3 pBoxEdges[3] = {D3DXVECTOR3(1,0,0), D3DXVECTOR3(0,1,0), D3DXVECTOR3(0,0,1)};
	for(int i=0;i<3;i++)
	{
		// edge up-down
		D3DXVECTOR3 vNorm1;
		//= Normalize(Cross(frustum.vPts[1] - frustum.vPts[0], pBoxEdges[i]));
		D3DXVec3Cross( &vNorm1, &(frustum.vPts[1] - frustum.vPts[0]), &pBoxEdges[i] );
		D3DXVec3Normalize( &vNorm1, &vNorm1 );
		if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm1)) return false;

		// edge left-right
		D3DXVECTOR3 vNorm2;
		//= Normalize(Cross(frustum.vPts[3] - frustum.vPts[0], pBoxEdges[i]));
		D3DXVec3Cross( &vNorm2, &(frustum.vPts[3] - frustum.vPts[0]), &pBoxEdges[i] );
		D3DXVec3Normalize( &vNorm2, &vNorm2 );
		if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm2)) return false;

		// edge bottom left
		D3DXVECTOR3 vNorm3;
		//= Normalize(Cross(frustum.vPts[4] - frustum.vPts[0], pBoxEdges[i]));
		D3DXVec3Cross( &vNorm3, &(frustum.vPts[4] - frustum.vPts[0]), &pBoxEdges[i] );
		D3DXVec3Normalize( &vNorm3, &vNorm3 );
		if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm3)) return false;

		// edge top left
		D3DXVECTOR3 vNorm4;
		//= Normalize(Cross(frustum.vPts[5] - frustum.vPts[1], pBoxEdges[i]));
		D3DXVec3Cross( &vNorm4, &(frustum.vPts[5] - frustum.vPts[1]), &pBoxEdges[i] );
		D3DXVec3Normalize( &vNorm4, &vNorm4 );
		if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm4)) return false;

		// edge top right
		D3DXVECTOR3 vNorm5;
		//= Normalize(Cross(frustum.vPts[6] - frustum.vPts[2], pBoxEdges[i]));
		D3DXVec3Cross( &vNorm5, &(frustum.vPts[6] - frustum.vPts[2]), &pBoxEdges[i] );
		D3DXVec3Normalize( &vNorm5, &vNorm5 );
		if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm5)) return false;

		// edge bottom right
		D3DXVECTOR3 vNorm6;
		//= Normalize(Cross(frustum.vPts[7] - frustum.vPts[3], pBoxEdges[i]));
		D3DXVec3Cross( &vNorm6, &(frustum.vPts[7] - frustum.vPts[3]), &pBoxEdges[i] );
		D3DXVec3Normalize( &vNorm6, &vNorm6 );
		if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm6)) return false;
	}

	// all tests passed - intersection occurs
	return true;
}


inline bool IntersectionTest(const CAABB &objectBB, const CAABB &frustumBB)
{
	// min and max vectors
	const D3DXVECTOR3 &vFrustumMin = frustumBB.vMin;
	const D3DXVECTOR3 &vFrustumMax = frustumBB.vMax;
	const D3DXVECTOR3 &vObjectMin = objectBB.vMin;
	const D3DXVECTOR3 &vObjectMax = objectBB.vMax;

	// test all axes
	//
	if(vObjectMin.x > vFrustumMax.x || vFrustumMin.x > vObjectMax.x) return false;
	if(vObjectMin.y > vFrustumMax.y || vFrustumMin.y > vObjectMax.y) return false;
	if(vObjectMin.z > vFrustumMax.z || vFrustumMin.z > vObjectMax.z) return false;

	// all tests passed - intersection occurs
	return true;
}


void OctreeSceneManager::Update(/*const CFrustum& frustum*/)
{
	m_Light.DoControls();

	std::list<CObject*>::iterator it = m_allObject.begin();	
	while ( it != m_allObject.end() )
	{
		CObject* sn = *it;
		sn->Update();
		++it;
	}

	//FindVisibleObjects(frustum);

	m_pRenderQueue->clear();

	CFrustum frustum;
	frustum = g_Camera.CalculateFrustum(g_Camera.m_fNearClip, g_Camera.m_fFarClip);
	//CAABB frustumAABB = frustum.m_AABB;
	it = m_allObject.begin();	
	while ( it != m_allObject.end() )
	{
		CObject* sn = *it;
		if ( sn && IntersectionTest(sn->m_WorldAABB,frustum.m_AABB) )
		{
			m_pRenderQueue->PushToNormalRQ(sn);
		}
		else
		{
			int i = 5;
		}

		++it;
	}
}


void OctreeSceneManager::walkOctree(const CFrustum& frustum,COctreeNode* octNode,CFrustum::Visble parentVisible)
{
	assert(octNode);
	if (octNode == NULL)
		return;

    if ( octNode->GetNumObject() == 0 )
        return;

	if (parentVisible == CFrustum::Visble::NONE) 
	{
		assert(false);
		return;
	}

	CFrustum::Visble visble = CFrustum::Visble::NONE;

	if (octNode == mOctree)
		visble = CFrustum::Visble::PARTIAL;
	else if (parentVisible == CFrustum::Visble::FULL)
		visble = CFrustum::Visble::FULL;
 	else if (parentVisible == CFrustum::Visble::PARTIAL)
 	{
 		CAABB box;
 		octNode->GetCullBounds(&box);
 		visble = frustum.VisibleTest(box);

		visble == CFrustum::Visble::PARTIAL;
 	}

    if (visble != CFrustum::NONE)
    {
        std::list<CObject*>::iterator it = octNode->m_ObjectList.begin();

        while ( it != octNode->m_ObjectList.end() )
        {
            CObject* sn = *it;

			//if ( visble == CFrustum::FULL || 
			//	 visble == CFrustum::PARTIAL && frustum.VisibleTest( sn->GetWorldAABB() ) != CFrustum::NONE )
			//{
				m_pRenderQueue->PushToNormalRQ(sn);
			//}
			
            ++it;
        }

		COctreeNode* child = NULL;
        if ( (child = octNode->m_pChildren[0][0][0]) != 0 )
            walkOctree( frustum, child, visble );

        if ( (child = octNode->m_pChildren[1][0][0]) != 0 )
            walkOctree( frustum, child, visble );

        if ( (child = octNode->m_pChildren[0][1][0]) != 0 )
            walkOctree( frustum, child, visble );

        if ( (child = octNode->m_pChildren[1][1][0]) != 0 )
            walkOctree( frustum, child, visble );

        if ( (child = octNode->m_pChildren[0][0][1]) != 0 )
            walkOctree( frustum, child, visble );

        if ( (child = octNode->m_pChildren[1][0][1]) != 0 )
            walkOctree( frustum, child, visble );

        if ( (child = octNode->m_pChildren[0][1][1]) != 0 )
            walkOctree( frustum, child, visble );

        if ( (child = octNode->m_pChildren[1][1][1]) != 0 )
            walkOctree( frustum, child, visble );

    }
}

void OctreeSceneManager::AddMapObject(const D3DXVECTOR3& pos,float scale,float angle,
									  const std::string& modelName,const std::string& hashName)
{
	CMapObj* mapObject = new CMapObj;
	mapObject->SetWordPos(pos);
	mapObject->SetScale(scale);
	mapObject->SetAngle(angle);
	mapObject->SetSkinMesh(modelName);
	mapObject->SetName(modelName);
	mapObject->UpdateWorldAABB();
	mapObject->SetHashName(hashName);
	mapObject->HashObject();

	AddObject(mapObject,mOctree);

	m_allObject.push_back(mapObject);
}

void OctreeSceneManager::AddCharactor(const D3DXVECTOR3& pos,float scale,float angle,
				  const std::string& modelName,const std::string& hashName)
{
	CCharactor* cha = new CCharactor;
	cha->m_ObjFlag |= FOB_IS_SKIN;
	cha->SetWordPos(pos);
	cha->SetScale(scale);
	cha->SetAngle(angle);
	cha->SetSkinMesh(modelName);
	cha->SetAni("idle");
	cha->SetName(modelName);
	cha->UpdateWorldAABB();
	cha->SetHashName(hashName);
	cha->HashObject();

	AddObject(cha,mOctree);

	m_allObject.push_back(cha);
}

void OctreeSceneManager::AddTerrainScetion(CTerrainSection* pTerrainSection)
{
	assert(pTerrainSection);
	if (pTerrainSection == NULL)
		return;

	//char strName[MAX_PATH] = {0};
	//_snprintf( strName, sizeof(strName), "TerrainSection_%d_%d", heightMapX, heightMapY );

	pTerrainSection->m_ObjFlag |= FOB_IS_TERRAIN;
	//pTerrainSection->m_ObjFlag |= FOB_NOT_CAST_SHADOW;
	pTerrainSection->SetHashName("TerrainSection_%d_%d");
	pTerrainSection->HashObject();
	g_SceneMng.AddObject(pTerrainSection,mOctree);
	g_SceneMng.m_allObject.push_back(pTerrainSection);
}


void OctreeSceneManager::SetSelfCha(CCharactor* cha) 
{
	m_pSelfCha = cha;

// 	D3DXVECTOR3 lookAtPos = cha->GetWordPos();
// 	D3DXVECTOR3 eyePos;
// 	eyePos.x = lookAtPos.x;
// 	eyePos.y = lookAtPos.y + 150;
// 	eyePos.z = lookAtPos.z - 150;
// 
// 	g_Camera.SetViewParams( eyePos, lookAtPos );

	//g_Camera.SetEyePt(eyePos);
	//g_Camera.SetLookAtPt( cha->GetWordPos() );
	//g_Camera.SetYaw( cha->GetAngle() - D3DX_PI / 2 );
	//g_Camera.SetPitch(- D3DX_PI / 3);
	//g_Camera.SetRadius(300);
}



void OctreeSceneManager::SetLightParams(const D3DXVECTOR3 &vLightPos, const D3DXVECTOR3 &vLightAt)
{
	m_Light.m_vSource = vLightPos;
	m_Light.m_vTarget = vLightAt;

	D3DXVECTOR3 vDir = m_Light.m_vSource - m_Light.m_vTarget;
	D3DXVec3Normalize(&vDir, &vDir);
	m_Light.m_ControlState.m_vRotation.x = atan2f(vDir.x, vDir.z);
	m_Light.m_ControlState.m_vRotation.y = asin(-vDir.y);

	m_Light.DoControls();
}

void OctreeSceneManager::InitLight()
{
	//m_fLightFov = D3DX_PI / 2.0f;

// 	m_Light.Diffuse.r = 1.0f;
// 	m_Light.Diffuse.g = 1.0f;
// 	m_Light.Diffuse.b = 1.0f;
// 	m_Light.Diffuse.a = 1.0f;
// 	m_Light.Position = D3DXVECTOR3( -8.0f, -8.0f, 0.0f );
// 	m_Light.Direction = D3DXVECTOR3( 1.0f, -1.0f, 0.0f );
// 	D3DXVec3Normalize( (D3DXVECTOR3*)&m_Light.Direction, (D3DXVECTOR3*)&m_Light.Direction );
// 	m_Light.Range = 10.0f;
// 	m_Light.Theta = m_fLightFov / 2.0f;
// 	m_Light.Phi = m_fLightFov / 2.0f;

	//D3DXMatrixPerspectiveFovLH( &m_matLightProj, m_fLightFov, 4 / 3.0f, 0.1f, 4000.0f );

	m_LightModel.LoadFromXFile("res\\sphere0.x");	

// 	std::vector<D3DXVECTOR3> pos;
// 	std::vector<int> index;
// 	CreateConeMesh( pos, index, 0, sqrt(3.0f) / 3, 1.0f, 12);

}




void OctreeSceneManager::CreateScene(std::string sScenName)
{
	TiXmlDocument sceneDoc;
	bool bIsLoad = sceneDoc.LoadFile(sScenName);
	assert(bIsLoad);
	if (!bIsLoad)
		return;

	TiXmlElement* root = sceneDoc.FirstChildElement();
	TiXmlElement* element = root->FirstChildElement( "Object" );
	while(element)
	{
		const char* strTemp = element->Attribute( "type" );
		if ( strcmp( "TerrainLiquid", strTemp ) == 0 )
		{
			TerrainLiquid* pTerrainLiquid = new TerrainLiquid;

			std::string materialName;
			D3DXVECTOR3 pos;
			float textureScale = 0.0f;
			bool bIsDepthEnable = false;
			float depthHeight = 0.0f;

			TiXmlElement* propriety = element->FirstChildElement( "Property" );
			while(propriety)
			{
				const char* strTemp = propriety->Attribute( "name" );
				const char* sValue = propriety->Attribute( "value" );

				if ( std::string("material") == std::string(strTemp)  )
				{
					sValue = UTF8ToANSI(sValue);
					materialName = sValue;
				}
				else if ( std::string("position") == std::string(strTemp) )
				{
					sscanf( sValue, "%f %f %f", &pos.x, &pos.y, &pos.z );
				}
				else if ( std::string("texture scale") == std::string(strTemp) )
				{
					sscanf( sValue, "%f", &textureScale );
				}
				else if ( std::string("depth texture layer.enable") == std::string(strTemp) )
				{
					if ( std::string(sValue) == std::string("true") )
						bIsDepthEnable = true;
					else
						bIsDepthEnable = false;
				}
				else if ( std::string("depth texture layer.height scale") == std::string(strTemp) )
				{
					sscanf( sValue, "%f", &depthHeight );
				}

				propriety = propriety->NextSiblingElement();
			}

			pTerrainLiquid->create( pos, textureScale, bIsDepthEnable, depthHeight, &( CTerrain::GetSingleton() ) );	

			m_pTerrainLiquids.push_back(pTerrainLiquid);

		}

		element = element->NextSiblingElement();
	}	

}



