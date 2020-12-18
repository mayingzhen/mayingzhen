#pragma once

namespace ma
{
	class RenderProxy;

	class CullNode : public Referenced
	{
	public:
		virtual void AddObject(RenderProxy* pObject) = 0;
		
		virtual void RemoveObject(RenderProxy* pObject) = 0;

		virtual void GetCullBoundingBox(AABB& box) const = 0;

		virtual uint32_t GetObjectAmount() const = 0;

		virtual RenderProxy* GetObjectByIndex( uint32_t nIndex ) const = 0;
	};


}


