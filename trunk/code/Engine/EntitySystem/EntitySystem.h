#ifndef  _EntitySystem__H__
#define  _EntitySystem__H__


namespace ma
{
	class GameObject;
	class IPhysicsScene;
	class Camera;
	class MeshComponent;
	class CullTree;


	class ENGINE_API EntitySystem 
	{
	
	public:

		void					Init();

		void					ShoutDown();

		void					Update();

		ref_ptr<GameObject>		CreateGameObject(const char* pName);

		//void					DeleteGameObject(GameObject*);

		//void					DeleteAll();
	
		void					Reset();

		void					Serialize(Serializer& sl, const char* pszLable = "Scene");

		CullTree*				GetCullTree() {return m_pCullTree.get();}

		ref_ptr<GameObject>		GetRootGameObject() {return m_pRootGameObject;}	

	private:
		ref_ptr<GameObject>		m_pRootGameObject;	

		ref_ptr<CullTree>		m_pCullTree;
	};

	ENGINE_API EntitySystem*	GetEntitySystem();

	ENGINE_API void				SetEntitySystem(EntitySystem* pEntitySystem);


}



#endif // _EntitySystem__H__
