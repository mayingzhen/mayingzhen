#ifndef __BulletCharacterController_H__
#define __BulletCharacterController_H__


namespace ma
{
	class BulletPhysicsObject;

	enum CollisionFlags
	{
		CF_None = 0,
		CF_Side,
		CF_Above,
		CF_Below,
	};

	class BULLETPHYSICS_API CharacterController : public Component , public btActionInterface
	{
		DECL_OBJECT(CharacterController)

	public:
		CharacterController();

		~CharacterController();

		static void							RegisterObject(Context* context);		

		//virtual void						SetCollisionShape(ICapsuleCollisionShape* pCapsule) = 0;

		virtual CapsuleCollisionShape*		GetCollisionShape();

		virtual void						SetStepOffset(float fStepOffset);

		virtual float						GetStepOffset();

		virtual bool						IsGrounded() const;	

		virtual btCollisionObject*			Start();

		virtual void						Stop();

		//void								MoveImpl(const Vector3& motion);

		virtual void						updateAction( btCollisionWorld* collisionWorld, btScalar deltaTimeStep);
		
		virtual void						debugDraw(btIDebugDraw* debugDrawer) {}

		void								DebugRender();

	private:
		bool								RecoverFromPenetration(int nMaxSubStep);


	private:
		btPairCachingGhostObject*			m_ghostObject;
		btCompoundShape*					m_collShape;
		
		float								m_fSetpOffset;

		CapsuleCollisionShape*				m_pCapsuleShape;

		float								m_touchSkin;	//Margin used for stay on ground
		bool								m_bTouched;
		Vector3								m_touchNorm;
		btManifoldArray						m_manifoldArray;

		CollisionFlags						m_colState;
	};


	inline bool IsCharacterController(const btCollisionObject* pObj)
	{
		if (pObj == NULL)
			return false;
		
		int collFlags = pObj->getCollisionFlags();
		return BitFieldBase<int>::StaticGetBit(collFlags,btCollisionObject::CF_CHARACTER_OBJECT);
	}
}


#endif
