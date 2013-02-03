#ifndef __IPhysicsCollisionShape_H__
#define __IPhysicsCollisionShape_H__


namespace ma
{
	class FRAMEWORK_API ICollisionShape
	{
	public:
		virtual void			SetTransformLS(const maNodeTransform& tsfLS) = 0;

		virtual maNodeTransform GetTransformLS() = 0;

		virtual	void			SetCollisionLayer(int nCollLayer) = 0;
		
		virtual int				GetCollisionLayer() = 0;
	};

	class FRAMEWORK_API IBoxCollisionShape : public ICollisionShape
	{
	public:
		virtual void			SetSize(const D3DXVECTOR3& vSize) = 0;

		virtual	D3DXVECTOR3		GetSize() = 0;
	};

	class FRAMEWORK_API ISphereCollisionShape : public ICollisionShape
	{
	public:
		virtual void			SetRadius(float fRadius) = 0;

		virtual	float			GetRadius() = 0;
	};
}

#endif

