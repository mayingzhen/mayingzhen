#ifndef  _CullNode__H__
#define  _CullNode__H__

namespace ma
{
	class RenderComponent;

	class CullNode : public Referenced
	{
	public:
		virtual void AddObject(RenderComponent* pObject) = 0;
		
		virtual void RemoveObject(RenderComponent* pObject) = 0;

		virtual void GetCullBoundingBox(AABB& box) const = 0;

		virtual UINT GetObjectAmount() const = 0;

		virtual RenderComponent* GetObjectByIndex( UINT nIndex ) const = 0;
	};


}

#endif
