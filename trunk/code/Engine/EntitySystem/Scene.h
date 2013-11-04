#ifndef  _SCENE__H__
#define  _SCENE__H__


namespace ma
{
	class GameObject;
	class IPhysicsScene;
	class Camera;


	class ENGINE_API Scene : public Object
	{
		DECL_OBJECT(Scene);

	public:
		Scene();

		~Scene();

		void			Update();

		void			Render();

		void			TravelScene(SceneVisiter* pVisiter);
 		
		void			Serialize(Serializer& sl, const char* pszLable = "Scene");

		void			AddSceneNode(SceneNode* pSceneNode);

		SceneNode*		CreateSceneNode(GameObject* pGameObj);

	private:
		SceneNode*		m_pRootNode;
	};

	ENGINE_API Scene*	GetSceneSystem();

	ENGINE_API void		SetSceneSystem(Scene* pScene);


}



#endif
