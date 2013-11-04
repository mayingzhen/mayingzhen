#ifndef  _Component__H__
#define  _Component__H__

namespace ma
{
	class GameObject;
	class SceneNode;

	class ENGINE_API Component : public Object
	{
		DECL_OBJECT(Component)

	public:
		Component(GameObject* pGameObj);

		virtual ~Component();

		virtual	void	UpdateTransform();

		GameObject*		GetGameObject();

		SceneNode*		GetSceneNode();

		virtual void	Serialize(Serializer& sl, const char* pszLable = "Component");

	protected:
		GameObject*		m_pGameObject;
		SceneNode*		m_pSceneNode;
	};
}



#endif // _Component__H__