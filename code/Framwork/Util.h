#ifndef  _GameObj_Util__H__
#define  _GameObj_Util__H__



namespace ma
{
	class GameObject;
	class Scene;

	FRAMWORK_API GameObject* CreateGameObjectWithCollision(Scene* pScene,const char* pSknPath,const char* pTexPath);
}



#endif // _GameObj_Util__H__
