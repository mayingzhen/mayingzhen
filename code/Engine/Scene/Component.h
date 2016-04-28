#ifndef  _Component__H__
#define  _Component__H__

#include "SceneNode.h"

namespace ma
{
	class SceneNode;
	class Camera;

	class Component : public Serializable
	{
		
	public:
		Component();

		virtual ~Component();

		DECL_OBJECT(Component);
		
		virtual	void	Update() {}
		virtual void	ParallelUpdate() {}
		virtual void	EndParallelUpdate() {}
		virtual void	ParallelShow(Camera* pCamera) {}

		virtual	void	OnAddToSceneNode(SceneNode* pNode) {}
		virtual	void	OnRemoveFromSceneNode(SceneNode* pNode) {}

		void			SetSceneNode(SceneNode* pSceneNode);	
		SceneNode*		GetSceneNode() const;

		virtual void	SetNeedChange(CHANGE_TYPE eChangeType);

	protected:
		SceneNode*		m_pSceneNode;

		int				m_nNeedChange;
	};

}



#endif // _Component__H__