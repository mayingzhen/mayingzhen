#ifndef  _Component__H__
#define  _Component__H__

namespace ma
{
	class GameObject;
	class SceneVisiter;
	class SceneNode;

	class ENTITYSYSTEM_API Component : public Object
	{
		DECL_OBJECT(Component)

	public:
		Component(GameObject* pGameObj);

		virtual ~Component();

		virtual void	Start();

		virtual void	Stop();

		virtual	void	Update();

		virtual void	Render();

		GameObject*		GetGameObject();

		SceneNode*		GetSceneNode();

		virtual void	Serialize(Serializer& sl, const char* pszLable = "Component") = 0;

	protected:
		GameObject* m_pGameObject;
	};
}



#endif // _Component__H__