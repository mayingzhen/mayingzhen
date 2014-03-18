#ifndef  _GameObj_Util__H__
#define  _GameObj_Util__H__



namespace ma
{
	class GameObject;
	class Scene;

	RENDER_API GameObjectPtr CreateMeshGameObject(const char* pSknPath,const char* pTexPath);

	RENDER_API GameObjectPtr CreateCollisionGameObject(const char* pSknPath,const char* pTexPath);
	
	RENDER_API GameObjectPtr CreateAnimationGameObject(const char* psknPath,const char* pTexPath,
		const char* pSkePth,const char* pAnimSetPath);
}



#endif // _GameObj_Util__H__
