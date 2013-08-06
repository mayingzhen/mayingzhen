#ifndef __IPhysicsCollisionShape_H__
#define __IPhysicsCollisionShape_H__


namespace ma
{
	class ENGINE_API ICollisionShape
	{
	public:
		virtual void			SetTransformLS(const NodeTransform& tsfLS) = 0;

		virtual NodeTransform	GetTransformLS() = 0;
	};

	class ENGINE_API IBoxCollisionShape : public ICollisionShape
	{
	public:
		virtual void			SetSize(const Vector3& vSize) = 0;

		virtual	Vector3			GetSize() = 0;
	};

	class ENGINE_API ISphereCollisionShape : public ICollisionShape
	{
	public:
		virtual void			SetRadius(float fRadius) = 0;

		virtual	float			GetRadius() = 0;
	};

	class ENGINE_API ICapsuleCollisionShape : public ICollisionShape
	{
	public:
		virtual void			SetHeight(float fHeight) = 0;

		virtual float			GetHeight() const = 0;

		virtual void			SetRadius(float fRadius) = 0;

		virtual float			GetRadius() const = 0;
	};
}

#endif

