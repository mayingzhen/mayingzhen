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
#include "HDRPostProcess.h"
#include "ShadowMap.h"

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

	//m_pRenderQueue = new CRenderQueue;


	//m_LightModel.LoadFromXFile("res\\sphere0.x");
}

OctreeSceneManager::~OctreeSceneManager()
{
    SAFE_DELETE(mOctree);
	//SAFE_DELETE(m_pRenderQueue);

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
}

const std::list<Light>& OctreeSceneManager::GetLigtList()
{
	return m_allLight;
}

std::list<ShadowMap*>& OctreeSceneManager::GetShadowMapList()
{
	return m_allShdowMap;
}

const std::vector<TerrainLiquid*> OctreeSceneManager::GetTerrainLiquidList()
{
	return m_pRenderLiquids;
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
	//m_pRenderQueue->clear();

	walkOctree(frustum,mOctree,CFrustum::Visble::PARTIAL);
}


std::list<CObject *> OctreeSceneManager::FindCasters(const CFrustum &frustum)
{
	std::list<CObject* > casters;
	std::list<CObject*>::iterator objit = m_allObject.begin();	
	for ( ; objit != m_allObject.end(); ++objit )
	{
		CObject *pObject = *objit;
		if(pObject->m_ObjFlag & FOB_NOT_CAST_SHADOW) 
			continue;

		//if ( pObject->m_WorldAABB.Intersect(frustum.m_AABB) )
		//	continue;

		casters.push_back(pObject);
	}
	return casters;
}

void OctreeSceneManager::Update()
{
	g_Camera.Update();

	std::list<CObject*>::iterator objit = m_allObject.begin();	
	while ( objit != m_allObject.end() )
	{
		CObject* sn = *objit;
		if (sn == NULL)
			continue;

		sn->Update();
		++objit;
	}

	CFrustum cameraFrustum = g_Camera.CalculateFrustum(g_Camera.m_fNearClip, g_Camera.m_fFarClip);
	
	///// water
	m_pRenderLiquids.clear();
	for (int i = 0; i < m_pTerrainLiquids.size(); ++i)
	{
		TerrainLiquid* pObject = m_pTerrainLiquids[i];
		if ( pObject && pObject->m_WorldAABB.Intersect(cameraFrustum.m_AABB) )
		{
			m_pRenderLiquids.push_back(pObject);
			continue;
		} 
	}

	///// shadow map
	CShadowMapPool::ClearAllUseFlag();
	m_allShdowMap.clear();

	std::list<Light>::iterator lightIt = m_allLight.begin();
	for (; lightIt != m_allLight.end(); ++lightIt )
	{
		lightIt->Update();
		lightIt->DoControls(); // test

		for (int i = 0; i < CCamera::NUM_PSSM; ++i)
		{
			CFrustum splitFrustum;
			splitFrustum = g_Camera.CalculateFrustum(g_Camera.m_fSplitPos[i], g_Camera.m_fSplitPos[i + 1]);
			std::list<CObject*> ObjList = FindCasters(splitFrustum); 

			if ( ObjList.empty() ) 
			{
				lightIt->m_pShadowMap[i] = NULL;
				continue;
			}
			else
			{
				ShadowMap* pShadowMap = CShadowMapPool::GetOneShdowMap();
				lightIt->m_pShadowMap[i] = pShadowMap;	
				m_allShdowMap.push_back(pShadowMap);

				D3DXMATRIX mCropMatrix = lightIt->CalculateCropMatrix(splitFrustum);
				pShadowMap->m_nIndex = i;
				pShadowMap->m_viewMat = lightIt->m_mView;
				pShadowMap->m_projMat = lightIt->m_mProj * mCropMatrix;
				pShadowMap->m_TexMat = pShadowMap->m_viewMat * pShadowMap->m_projMat * *( pShadowMap->GetTexScaleBiasMat() );
				pShadowMap->m_casterList.clear();
				pShadowMap->m_casterList = ObjList;
			}			
		}
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

		//visble = CFrustum::Visble::PARTIAL;
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
				//m_pRenderQueue->PushToNormalRQ(sn);
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

	char strName[MAX_PATH] = {0};
	_snprintf( strName, sizeof(strName), "TerrainSection_%d_%d",
		pTerrainSection->GetHeightMapX(), pTerrainSection->GetHeightMapY() );

	pTerrainSection->m_ObjFlag |= FOB_IS_TERRAIN;
	//pTerrainSection->m_ObjFlag |= FOB_NOT_CAST_SHADOW;
	pTerrainSection->SetHashName(strName);
	pTerrainSection->HashObject();
	g_SceneMng.AddObject(pTerrainSection,mOctree);
	g_SceneMng.m_allObject.push_back(pTerrainSection);
}

void OctreeSceneManager::AddTerrainLiquid(TerrainLiquid* pTerrainLiquid)
{
	assert(pTerrainLiquid);
	if (pTerrainLiquid == NULL)
		return;

	pTerrainLiquid->m_ObjFlag |= FOB_NOT_CAST_SHADOW;
	pTerrainLiquid->SetHashName("TerrainLiquid");
	pTerrainLiquid->HashObject();
	g_SceneMng.AddObject(pTerrainLiquid, mOctree);
	//g_SceneMng.m_allObject.push_back(pTerrainLiquid);	
	m_pTerrainLiquids.push_back(pTerrainLiquid);
}

void OctreeSceneManager::AddLight(const D3DXVECTOR3 &vLightPos, const D3DXVECTOR3 &vLightAt)
{
	Light addLight;
	addLight.m_vSource = vLightPos;
	addLight.m_vTarget = vLightAt;

	D3DXVECTOR3 vDir = addLight.m_vSource - addLight.m_vTarget;
	D3DXVec3Normalize(&vDir, &vDir);
	addLight.m_ControlState.m_vRotation.x = atan2f(vDir.x, vDir.z);
	addLight.m_ControlState.m_vRotation.y = asin(-vDir.y);
	
	m_allLight.push_back(addLight);
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

			TerrainLiquid* pTerrainLiquid = new TerrainLiquid;
			pTerrainLiquid->create( pos, textureScale, bIsDepthEnable, depthHeight, &( CTerrain::GetSingleton() ) );	
			AddTerrainLiquid(pTerrainLiquid);
		}

		element = element->NextSiblingElement();
	}	

}



