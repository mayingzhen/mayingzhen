#pragma once



#include <list>
#include <algorithm>
#include <string>

#include "OctreeNode.h"
#include "Camera.h"
#include "RenderQueue.h"
#include "Bounding.h"
#include "effect_file.h"
#include "SinMesh.h"
#include "light.h"
#include "ShadowMap.h"
#include "ScreenQuad.h"


class CCharactor;
class TerrainLiquid;
class CTerrainSection;


class OctreeSceneManager
{

public:
	OctreeSceneManager();
    ~OctreeSceneManager();

    void Init(CAABB &box, int depth);
	void ReleaseAll();


	const std::list<CObject*>& GetRenderObjList();

	void Update(/*const CFrustum& frustum*/);
	void Render();

	void AddMapObject(const D3DXVECTOR3& pos,float scale,float angle,
		const std::string& modelName,const std::string& hashName); 
	void AddCharactor(const D3DXVECTOR3& pos,float scale,float angle,
		const std::string& modelName,const std::string& hashName);
	void AddTerrainScetion(CTerrainSection* pTerrainSection);

	CCharactor* GetSelfCha() {return m_pSelfCha;}
	void SetSelfCha(CCharactor* cha); 


	void InitLight();
	void SetLightParams(const D3DXVECTOR3 &vLightPos, const D3DXVECTOR3 &vLightAt);

	void CreateScene(std::string sScenName);
	
private:

	void FindVisibleObjects(const CFrustum& frustum);
	void walkOctree(const CFrustum& frustum,COctreeNode* octNode,CFrustum::Visble parentVisible);

	void UpdateObject(CObject* object); // 更新物体在八叉树中的位置
	void RemoveObject(CObject* object); // 将物体从它所属的八叉树节点移除 
	void AddObject(CObject *object, COctreeNode *octree, int depth = 0); // 将物体加到八叉树节点下(会找到尽可能深的子节点) 

public:

	struct SimpleVertex
	{
		SimpleVertex() {}
		SimpleVertex(const D3DXVECTOR3& position, const D3DXVECTOR2& texCoords)
		{
			this->position = position;
			this->texCoords = texCoords;
		}

		/// Vertex position
		D3DXVECTOR3 position;

		/// Vertex texture coordinate
		D3DXVECTOR2 texCoords;
	};
	
	CCharactor* m_pSelfCha; //自己

	CRenderQueue* m_pRenderQueue;

	std::list<CObject*> m_allObject;

	Light m_Light;
	CSkinMesh		   m_LightModel;

    COctreeNode *mOctree;     // The root octree

    int mMaxDepth;			// Max depth for the tree
    CAABB mBox;    // Size of the octree

	//IDirect3DVertexDeclaration9* m_simpleVertexDeclaration;
	//SimpleVertex m_quadVerts[4]; 
	

	std::vector<TerrainLiquid*> m_pTerrainLiquids;

};

extern OctreeSceneManager g_SceneMng;
