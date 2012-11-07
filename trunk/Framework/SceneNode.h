#ifndef  _SCENE_NODE__H__
#define  _SCENE_NODE__H__

#include "Framework/Object.h"

namespace ma
{
	class SceneNode : public Object
	{
	public:
		SceneNode(const char* pNodeName = NULL);
		virtual ~SceneNode();

		const D3DXMATRIX& GetWorldMatrix();

		D3DXVECTOR3 GetWorldPos();
		
		void SetWorldPos(const D3DXVECTOR3& pos);

		void SyncWorld();
	};

}


#endif