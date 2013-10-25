#ifndef  _GameObj_Util__H__
#define  _GameObj_Util__H__



namespace ma
{
	class GameObject;
	class Scene;

	ENGINE_API GameObject* CreateMeshGameObject(const char* pSknPath,const char* pTexPath);

	ENGINE_API GameObject* CreateCollisionGameObject(const char* pSknPath,const char* pTexPath);
	
	ENGINE_API GameObject* CreateAnimationGameObject(const char* psknPath,const char* pTexPath,const char* pSkePth);
}



#endif // _GameObj_Util__H__
