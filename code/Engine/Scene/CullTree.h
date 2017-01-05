#ifndef  _CullTree__H__
#define  _CullTree__H__

namespace ma
{
	class RenderComponent;
	class Frustum;
	class Camera;

	class CullTreeQuery
	{
	public:
		virtual Frustum::Visibility TestCullNode(CullNode* pCullNode) = 0;

		virtual void VisitCullNode(CullNode* pCullNode,Frustum::Visibility eNodeVisib) = 0;
	};

	class CullTree : public Referenced
	{
	public:
		// resize the culltree to the given size
		virtual void Resize(const AABB& box) = 0;

		// object operation
		virtual void AddObject(RenderComponent* pObject) = 0;
		
		virtual void RemoveObject(RenderComponent* pObject) = 0;
		
		virtual void UpdateObject(RenderComponent* pobject) = 0;

		virtual void FindObjectsIn(const Frustum* pFrustum,uint32 mask, OUT vector<RenderComponent*>& vecObj) = 0;

		virtual void FindObjectsIn(CullTreeQuery& query,uint32 mask) = 0;
	};
}

#endif
