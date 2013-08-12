#ifndef  _EntitySystem__H__
#define  _EntitySystem__H__


namespace ma
{
	class GameObject;
	class IPhysicsScene;
	class Camera;
	class MeshComponent;


	class FRAMWORK_API EntitySystem 
	{
	
	public:

		void			Init();

		void			ShoutDown();

		void			Start();
		 
		void			Stop();

		void			Update();

		//void			Render();

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

	FRAMWORK_API EntitySystem*	GetEntitySystem();

	FRAMWORK_API void			SetEntitySystem(EntitySystem* pEntitySystem);


}



#endif // _EntitySystem__H__
