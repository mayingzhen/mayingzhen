#ifndef  _EntitySystem__H__
#define  _EntitySystem__H__


namespace ma
{
	class GameObject;
	class IPhysicsScene;
	class Camera;
	class MeshComponent;


	class ENGINE_API EntitySystem 
	{
	
	public:

		void			Init();

		void			ShoutDown();

		void			Update();

		GameObject*		GetCameraObject() {return m_pCameraObject;}

		Camera*			GetCamera() {return m_pCamera;}

		GameObject*		CreateGameObject(const char* pName);

		void			DeleteGameObject(GameObject*);

		void			DeleteAll();

		void			Serialize(Serializer& sl);

	private:
		Scene*						m_pScene;

		std::vector<GameObject*>	m_arrGameObject;

		GameObject*					m_pCameraObject;
		Camera*						m_pCamera;
	};

	ENGINE_API EntitySystem*	GetEntitySystem();

	ENGINE_API void			SetEntitySystem(EntitySystem* pEntitySystem);


}



#endif // _EntitySystem__H__
