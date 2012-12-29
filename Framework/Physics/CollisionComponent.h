#ifndef __CollisionComponent_H__
#define __CollisionComponent_H__


namespace ma
{

	class FRAMEWORK_API CollisionComponent : public Component
	{
	public:
		CollisionComponent();

		~CollisionComponent();

		virtual maNodeTransform GetTransformLS() {return m_tsfLS;}

		virtual void SetTransformLS(const maNodeTransform& tsfLS) {m_tsfLS = tsfLS;}

	protected:
		maNodeTransform m_tsfLS;
	};

	class FRAMEWORK_API BoxCollisionComponent : public CollisionComponent
	{
	public:
		BoxCollisionComponent();

		~BoxCollisionComponent();

		virtual void Start();	

		D3DXVECTOR3 GetSize(){return m_vSize;}

		void SetSize(const D3DXVECTOR3& vSize) {m_vSize = vSize;}

	private:
		D3DXVECTOR3 m_vSize;

		IPhysicsObject* m_pPhysicsObj;
	};

}