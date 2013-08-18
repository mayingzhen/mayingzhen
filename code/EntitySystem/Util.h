#ifndef  _GameObj_Util__H__
#define  _GameObj_Util__H__



namespace ma
{
	class GameObject;
	class Scene;

	ENTITYSYSTEM_API GameObject* CreateMeshGameObject(const char* pSknPath,const char* pTexPath);

	ENTITYSYSTEM_API GameObject* CreateCollisionGameObject(const char* pSknPath,const char* pTexPath);
	
	ENTITYSYSTEM_API GameObject* CreateAnimationGameObject(const char* psknPath,const char* pTexPath,const char* pSkePth);
}



#endif // _GameObj_Util__H__
