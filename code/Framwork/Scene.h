#ifndef  _SCENE__H__
#define  _SCENE__H__


namespace ma
{
	class GameObject;
	class IPhysicsScene;
	class Camera;


	class FRAMWORK_API Scene : public Object
	{
		DECL_OBJECT(Scene);

	public:
		Scene();

		~Scene();

		void			TravelScene(SceneVisiter* pVisiter);
		
 		void			Update(float fElapsedTime);
 		
 		void			Render(Camera* pCmera = NULL);
	
		void			Awak();

 		void			Start();
 
 		void			Stop();

		virtual void	Serialize(Serializer& sl, const char* pszLable = "Scene");

		IPhysicsScene*	GetPhysicsScene() {return m_pPhyScene;}

		Camera*			GetCurCamera() {return m_pCurCamera;}

		GameObject*		CreateGameObject(const char* pName);

	private:
		SceneNode*				m_pRootNode;

		IPhysicsScene*			m_pPhyScene;

		Camera*					m_pCurCamera;
	};




}



#endif
