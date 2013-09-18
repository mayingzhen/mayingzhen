#ifndef  _EntitySystem__H__
#define  _EntitySystem__H__


namespace ma
{
	class GameObject;
	class IPhysicsScene;
	class Camera;
	class MeshComponent;


	class ENTITYSYSTEM_API EntitySystem 
	{
	
	public:

		void			Init();

		void			ShoutDown();

		void			Start();
		 
		void			Stop();

		void			Update();

		GameObject*		CreateGameObject(const char* pName);

		void			DeleteGameObject(GameObject*);

		void			DeleteAll();

		void			Serialize(Serializer& sl);

	private:
		Scene*						m_pScene;

		std::vector<GameObject*>	m_arrGameObject;

		// cach
		std::vector<MeshComponent*>	m_arrMeshComp;
	};

	ENTITYSYSTEM_API EntitySystem*	GetEntitySystem();

	ENTITYSYSTEM_API void			SetEntitySystem(EntitySystem* pEntitySystem);


}



#endif // _EntitySystem__H__
