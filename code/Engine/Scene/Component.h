#ifndef  _Component__H__
#define  _Component__H__

namespace ma
{
	class SceneNode;
	class NodeTransform;
	class Camera;

	class ENGINE_API Component : public Object
	{
		DECL_OBJECT(Component)

	public:
		Component(SceneNode* pGameObj);

		virtual ~Component();

		virtual	void	OnAddToSceneNode(SceneNode* pGameObj);

		virtual	void	OnRemoveFromSceneNode(SceneNode* pGameObj);

		virtual void	OnTransformChange() {}

		SceneNode*		GetSceneNode();

		virtual void	Serialize(Serializer& sl, const char* pszLable = "Component");

	protected:
		SceneNode*		m_pSceneNode;
	};

	DeclareRefPtr(Component);
}



#endif // _Component__H__