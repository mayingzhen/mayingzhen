#ifndef  _GameObj_Util__H__
#define  _GameObj_Util__H__



namespace ma
{
	class SceneNode;
	class Scene;

	RENDER_API SceneNode* CreateMeshGameObject(const char* pSknPath,const char* pTexPath);

	RENDER_API SceneNode* CreateCollisionGameObject(const char* pSknPath,const char* pTexPath);
	
	RENDER_API SceneNode* CreateAnimationGameObject(const char* psknPath,const char* pTexPath,
		const char* pSkePth,const char* pAnimSetPath);
}



#endif // _GameObj_Util__H__
