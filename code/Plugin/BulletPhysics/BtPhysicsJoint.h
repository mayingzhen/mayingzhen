#ifndef  _BtPhysicsJoint__H__
#define  _BtPhysicsJoint__H__

namespace ma
{

	class PhysicsJoint : public Component
	{
		
	public: 
		DECL_OBJECT(PhysicsJoint)

		static void		RegisterAttribute();		
		
		float			GetBreakingImpulse() {return m_fBreakingImpulse;} 
		void			SetBreakingImpulse(float impulse);

		bool			IsEnabled() const {return m_bEnabled;} 
		void			SetEnabled(bool enabled);

		void			SetATransformLS(const Transform& tsfLS){m_AtsfLS = tsfLS;}
		Transform		GetATransformLS() {return m_AtsfLS;} 
		
		Transform		GetBTransformLS() {return m_BtsfLS;} 
		void			SetBTransformLS(const Transform& tsfLS) {m_BtsfLS = tsfLS;}
		
		void			SetPysicsObjectB(SceneNode* pPhyObjB) {m_pPhyObjB = pPhyObjB;}

		btTypedConstraint*	GetBtConstraint() {return m_pConstraint;}
		
		virtual void	Start();

		virtual void	Stop();

	protected: 
		SceneNode*				m_pPhyObjB;
		btTypedConstraint*		m_pConstraint;
		float					m_fBreakingImpulse;
		bool					m_bEnabled;
		Transform				m_AtsfLS;
		Transform				m_BtsfLS;
	};


	class PhysicsGenericJoint : public PhysicsJoint
	{
	
	public:
		PhysicsGenericJoint();

		DECL_OBJECT(PhysicsGenericJoint)

		static void		RegisterAttribute();	

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

	class PhysicsHingeJoint : public PhysicsJoint
	{
		DECL_OBJECT(PhysicsHingeJoint)

	public:
		PhysicsHingeJoint();

		static void		RegisterAttribute();	

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