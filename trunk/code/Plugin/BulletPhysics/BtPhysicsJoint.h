#ifndef  _BtPhysicsJoint__H__
#define  _BtPhysicsJoint__H__

namespace ma
{

#define  DECL_PhysicsJoint \
	public : \
		virtual float			GetBreakingImpulse() {return m_fBreakingImpulse;} \
		virtual bool			IsEnabled() const {return m_bEnabled;} \
		virtual Transform	GetATransformLS() {return m_AtsfLS;} \
		virtual Transform	GetBTransformLS() {return m_BtsfLS;} \
		virtual void			SetBreakingImpulse(float impulse)\
		{ \
			m_fBreakingImpulse = impulse; \
			if (m_pConstraint) \
				m_pConstraint->setBreakingImpulseThreshold(impulse);\
		}\
		virtual void			SetEnabled(bool enabled)\
		{\
			m_bEnabled = enabled;\
			if (m_pConstraint)\
				m_pConstraint->setEnabled(enabled);\
		}\
		virtual void			SetATransformLS(const Transform& tsfLS)\
		{\
			m_AtsfLS = tsfLS;\
		}\
		virtual void			SetBTransformLS(const Transform& tsfLS)\
		{\
			m_BtsfLS = tsfLS;\
		}\
		virtual void			SetPysicsObjectB(SceneNode* pPhyObjB)\
		{\
			m_pPhyObjB = pPhyObjB; \
		}\
		btTypedConstraint*		GetBtConstraint() {return m_pConstraint;} \
	private: \
		SceneNode*				m_pPhyObjB;\
		btTypedConstraint*		m_pConstraint;\
		float					m_fBreakingImpulse;\
		bool					m_bEnabled;\
		Transform			m_AtsfLS;\
		Transform			m_BtsfLS;\
    private: 


	class BulletPhysicsGenericJoint : public IPhysicsGenericJoint
	{
		DECL_PhysicsJoint

	public:
		BulletPhysicsGenericJoint(SceneNode* pGameObj);

		virtual void	SetAngularLowerLimit(const Vector3& limit); 

		virtual void	SetAngularUpperLimit(const Vector3& limit);

		virtual void	SetLinearLowerLimit(const Vector3& limit);

		virtual void	SetLinearUpperLimit(const Vector3& limit);

		void			Start();

		void			Stop();

	private:
		Vector3			m_vAngularLowerLimit;
		Vector3			m_vAngularUpperLimit;
		Vector3			m_vLinearLowerLimit;
		Vector3			m_vLinearUpperLimit;

		bool			m_bAngularLowerLimit;
		bool			m_bAngularUpperLimit;
		bool			m_bLinearLowerLimit;
		bool			m_bLinearUpperLimit;
	};

	class BulletPhysicsHingeJoint : public IPhysicsHingeJoint
	{
		DECL_PhysicsJoint

	public:
		BulletPhysicsHingeJoint(SceneNode* pGameObj);

		virtual void	SetLimits(float minAngle, float maxAngle, float bounciness = 1.0f);

		void			Start();

		void			Stop();

	private:
		float			m_fMinAngle;
		float			m_fMaxAngle;
		float			m_fBounciness;
	};

}


#endif