#ifndef  _SCENE__H__
#define  _SCENE__H__


namespace ma
{
	class GameObject;
	class IPhysicsScene;
	class Camera;


	class ENTITYSYSTEM_API Scene : public Object
	{
		DECL_OBJECT(Scene);

	public:
		Scene();

		~Scene();

		void			TravelScene(SceneVisiter* pVisiter);
		
 		void			Update(float fElapsedTime);
//  		
//  		void			Render(Camera* pCmera = NULL);
	
// 		void			Awak();
// 
  		void			Start();
  
  		void			Stop();

		virtual void	Serialize(Serializer& sl, const char* pszLable = "Scene");

		Camera*			GetCurCamera() {return m_pCurCamera;}

		//GameObject*		CreateGameObject(const char* pName);
		//SceneNode*		GetRootNode() {return m_pRootNode;}
		void			AddSceneNode(SceneNode* pSceneNode);

		//void			DeleteGameObject(GameObject* pGameObj);

	private:
		SceneNode*				m_pRootNode;

		Camera*					m_pCurCamera;
	};




}



#endif
