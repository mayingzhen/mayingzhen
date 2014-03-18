#ifndef  _CullTree__H__
#define  _CullTree__H__

namespace ma
{
	class GameObject;
	class Frustum;
	class Camera;

	class ENGINE_API CullTree : public Referenced
	{
	public:
		// resize the culltree to the given size
		virtual void Resize(const AABB& box) = 0;

		// object operation
		virtual void AddObject(GameObject* pObject) = 0;
		
		virtual void RemoveObject(GameObject* pObject) = 0;
		
		virtual void UpdateObject(GameObject* pobject) = 0;

		// Recurses the tree, adding any obj that intersectig with the frustum/ray/box/sphere/volume into the given list
		virtual void FindObjectsIn(const Frustum* pFrustum, std::vector<GameObject*>& vecObj) const = 0;
		
		//virtual void FindObjectsIn(const AABB& box, std::vector<GameObject*>& vecObj) const = 0;
		
		//virtual void FindObjectsIn(const Sphere& sphere, OUT vector<CNode*>& vecObj) const = 0;
		
		//virtual void FindObjectsIn(const Ray& ray, OUT vector<std::pair<CNode*, float> >& vecObj) const = 0;
	};
	
//	ENGINE_API CullTree*	GetCullTree();
// 
// 	ENGINE_API void			SetCullTree(CullTree* pCullTree);	
}

#endif
