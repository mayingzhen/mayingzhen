#ifndef  _GameObj_Util__H__
#define  _GameObj_Util__H__



namespace ma
{
	class GameObject;
	class Scene;

	FRAMWORK_API GameObject* CreateCollisionGameObject(const char* pSknPath,const char* pTexPath);
	
	FRAMWORK_API GameObject* CreateAnimationGameObject(const char* psknPath,const char* pTexPath,const char* pSkePth);
}



#endif // _GameObj_Util__H__
