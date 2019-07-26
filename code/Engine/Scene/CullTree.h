#pragma once

namespace ma
{
	class RenderComponent;
	class Frustum;
	class Camera;

	class CullTree : public Referenced
	{
	public:
		// resize the culltree to the given size
		virtual void Resize(const AABB& box) = 0;

		// object operation
		virtual void AddObject(RenderComponent* pObject) = 0;
		
		virtual void RemoveObject(RenderComponent* pObject) = 0;
		
		virtual void UpdateObject(RenderComponent* pobject) = 0;

		virtual void FindObjectsIn(const Frustum* pFrustum, uint32_t mask, OUT vector<RenderComponent*>& vecObj) = 0;
	};
}

