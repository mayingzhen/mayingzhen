#pragma once

namespace ma
{
	class RenderProxy;
	class Frustum;
	class Camera;

	class CullTree : public Referenced
	{
	public:
		// resize the culltree to the given size
		virtual void Resize(const AABB& box) = 0;

		// object operation
		virtual void AddObject(RenderProxy* pObject) = 0;
		
		virtual void RemoveObject(RenderProxy* pObject) = 0;
		
		virtual void UpdateObject(RenderProxy* pobject) = 0;

		virtual void FindObjectsIn(const Frustum* pFrustum, uint32_t mask, OUT vector<RenderProxy*>& vecObj) = 0;
	};
}

