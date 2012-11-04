#ifndef  _SCENE_NODE__H__
#define  _SCENE_NODE__H__

namespace ma
{
	class SceneNode
	{
	public:
		SceneNode();
		virtual ~SceneNode();

		const D3DXMATRIX& GetWorldMatrix();

		D3DXVECTOR3 GetWorldPos();
		
		void SetWorldPos(const D3DXVECTOR3& pos);
	
		virtual void Render();

		virtual void Update();

		void AddChildNode(SceneNode* pSceneNode);

	private:
		std::vector<SceneNode*> m_vChildNodes;

	};

}


#endif