#ifndef  _SCENE_NODE__H__
#define  _SCENE_NODE__H__

#include "Framework/Object.h"

namespace ma
{
	class Scene;

	class SceneNode : public Object
	{
	public:
		SceneNode(Scene* pSene = NULL, SceneNode* pParentNode = NULL, const char* pNodeName = NULL);

		~SceneNode();

		void TranslateLS(const D3DXVECTOR3& vDeltaLS);

		void TranslatePS(const D3DXVECTOR3& vDeltaPS);

		void TranslateWS(const D3DXVECTOR3& vDeltaWS);

		maNodeTransform GetTransformWS();
		
		void SetTransformPS(const maNodeTransform& TSFPS) {m_tsfPS = TSFPS;}
		
		const maNodeTransform& GetTransformPS() {return m_tsfPS;}

		const D3DXMATRIX& GetWorldMatrix() {return m_matWorld;}

		const D3DXVECTOR3* GetWorldPos() {return (const D3DXVECTOR3*)(&m_matWorld._41);}
	
		void SyncWorld();

		Scene* GetScene() {return m_pScen;}

	private:
		std::vector<SceneNode*> m_vChildNodes;

		Scene* m_pScen;
		
		maNodeTransform m_tsfPS; // relate to parent node
		D3DXMATRIX m_matWorld;    // relate to world
	};

}


#endif