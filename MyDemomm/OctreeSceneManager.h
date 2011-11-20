#ifndef C_OCTREE_SCENE_MANAGGER__HH__
#define C_OCTREE_SCENE_MANAGGER__HH__

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
#include "ScreenQuad.h"

class CCharactor;
class TerrainLiquid;
class CTerrainSection;
class ShadowMap;


class OctreeSceneManager
{

public:
	OctreeSceneManager();
    ~OctreeSceneManager();

    void Init(CAABB &box, int depth);
	void ReleaseAll();

	const std::list<CObject*>& GetRenderObjList();
	const std::list<Light>& GetLigtList();
	std::list<ShadowMap*>& GetShadowMapList();
	const std::vector<TerrainLiquid*> GetTerrainLiquidList();

	void Update();

	void AddMapObject(const D3DXVECTOR3& pos,float scale,float angle,
		const std::string& modelName,const std::string& hashName); 
	void AddCharactor(const D3DXVECTOR3& pos,float scale,float angle,
		const std::string& modelName,const std::string& hashName);
	void AddTerrainScetion(CTerrainSection* pTerrainSection);
	void AddTerrainLiquid(TerrainLiquid* pTerrainLiquid);

	void AddLight(const D3DXVECTOR3 &vLightPos, const D3DXVECTOR3 &vLightAt);

	void CreateScene(std::string sScenName);
	
private:

	std::list<CObject *> FindCasters(const CFrustum &frustum);

	void FindVisibleObjects(const CFrustum& frustum);
	void walkOctree(const CFrustum& frustum,COctreeNode* octNode,CFrustum::Visble parentVisible);
	void UpdateObject(CObject* object); // 更新物体在八叉树中的位置
	void RemoveObject(CObject* object); // 将物体从它所属的八叉树节点移除 
	void AddObject(CObject *object, COctreeNode *octree, int depth = 0); // 将物体加到八叉树节点下(会找到尽可能深的子节点) 

private:

	//CRenderQueue* m_pRenderQueue;

	std::list<CObject*> m_allObject;
	std::list<Light> m_allLight;

	std::list<ShadowMap*> m_allShdowMap;

	CSkinMesh	m_LightModel;

    COctreeNode *mOctree;     // The root octree

    int mMaxDepth;			// Max depth for the tree
    CAABB mBox;				// Size of the octree
	
	std::vector<TerrainLiquid*> m_pRenderLiquids;  // need render Liquids
	std::vector<TerrainLiquid*> m_pTerrainLiquids; // all Liquids
};

extern OctreeSceneManager g_SceneMng;

#endif
