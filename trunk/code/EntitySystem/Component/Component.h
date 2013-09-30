#ifndef  _Component__H__
#define  _Component__H__

namespace ma
{
	class GameObject;

	class ENTITYSYSTEM_API Component : public Object
	{
		DECL_OBJECT(Component)

	public:
		Component(GameObject* pGameObj);

		virtual ~Component();

		virtual	void	BuildRenderItem();

		GameObject*		GetGameObject();

		SceneNode*		GetSceneNode();

		virtual void	Serialize(Serializer& sl, const char* pszLable = "Component") = 0;

	protected:
		GameObject*		m_pGameObject;
	};
}



#endif // _Component__H__