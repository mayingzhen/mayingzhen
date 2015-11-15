#ifndef  _Component__H__
#define  _Component__H__

namespace ma
{
	class SceneNode;
	class Camera;

	class Component : public Object
	{
		
	public:
		Component();

		virtual ~Component();

		DECL_OBJECT(Component);
		
		virtual	void	Update() {}
		virtual void	ParallelUpdate() {}
		virtual void	EndParallelUpdate() {}

		virtual	void	OnAddToSceneNode(SceneNode* pNode) {}
		virtual	void	OnRemoveFromSceneNode(SceneNode* pNode) {}

		void			SetSceneNode(SceneNode* pSceneNode);	
		SceneNode*		GetSceneNode() const;

		virtual void	MarkDirty() {}

	protected:
		SceneNode*		m_pSceneNode;
	};

}



#endif // _Component__H__