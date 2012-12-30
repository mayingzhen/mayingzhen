#ifndef  _IPhysicsObject__H__
#define  _IPhysicsObject__H__

namespace ma
{

	struct COMMON_API RigidBodyInfo
	{
		UINT		m_collisionFlags;
		bool		m_bUseGravity;
		bool		m_bKinematic;
		float		m_fMass;
		float		m_fLinearDamping;
		float		m_fAngularDamping;
		void*		m_UserData;
	};

	struct COMMON_API BoxCollisionInfo 
	{
		maNodeTransform m_tsfLS;
		D3DXVECTOR3 m_vSize;

		BoxCollisionInfo(const maNodeTransform& tsfLS,const D3DXVECTOR3& vSize)
		{
			m_tsfLS = tsfLS;
			m_vSize = vSize;
		}
	};


	class COMMON_API IPhysicsObject
	{
	public:
		virtual void Start() = 0;

		virtual void Stop() = 0;

		virtual bool IsKinematic() = 0;

		virtual void SetRigidBody(const RigidBodyInfo& info) = 0;

		virtual void AddBoxCollision(const BoxCollisionInfo& info) = 0;

		virtual void SetTransformWS(const maNodeTransform& tsfWS) = 0;

		virtual maNodeTransform GetTransformWS() const = 0;
	};
}


#endif
