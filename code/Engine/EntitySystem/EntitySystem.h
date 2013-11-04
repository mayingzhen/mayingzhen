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

		GameObject*		CreateGameObject(const char* pName);

		void			DeleteGameObject(GameObject*);

		void			DeleteAll();

	private:

		std::vector<GameObject*>	m_arrGameObject;
	};

	ENGINE_API EntitySystem*	GetEntitySystem();

	ENGINE_API void			SetEntitySystem(EntitySystem* pEntitySystem);


}



#endif // _EntitySystem__H__
