#ifndef __CollisionComponent_H__
#define __CollisionComponent_H__


namespace ma
{
	class ICollisionShape;
	class IBoxCollisionShape;
	class ISphereCollisionShape;
	class IPhysicsObject;

	enum CollisionLayer
	{
		ColLayer_Default = 1,
		ColLayer_Terrain = 2,
	};


	class FRAMEWORK_API CollisionComponent : public Component
	{
	public:
		CollisionComponent(); 

		//virtual ~CollisionComponent() = 0;

		maNodeTransform GetTransformLS() const; 

		void SetTransformLS(const maNodeTransform& tsfLS); 

		void SetCollisionLayer(int eCollLayer);

		int	GetCollisionLayer();

		virtual void SetGameObject(GameObject* pGameObj);

	protected:
		D3DXMATRIX GetWorldMatrix();

	protected:
		ICollisionShape* m_pCollisionShape;

		IPhysicsObject* m_pPhysicsObject;
	};


	class FRAMEWORK_API BoxCollisionComponent : public CollisionComponent
	{
	public:
		BoxCollisionComponent();

		void		SetSize(const D3DXVECTOR3& vSize);

		D3DXVECTOR3	GetSize() const; 

		virtual void DbgRender(BitField flag);

		virtual void SetGameObject(GameObject* pGameObj);

	private:
		IBoxCollisionShape* m_pBoxCollisionShape;
	};


	class FRAMEWORK_API SphereCollisionComponent : public CollisionComponent
	{
	public:
		SphereCollisionComponent();

		void		SetRadius(float fRadius);

		float		GetRadius() const;

		virtual void SetGameObject(GameObject* pGameObj);

		virtual void DbgRender(BitField flag);

	private:
		ISphereCollisionShape* m_pSphereCollisionShape;
	};

}

#endif
