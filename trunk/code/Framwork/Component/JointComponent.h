#ifndef __JointComponent_H__
#define __JointComponent_H__

namespace ma
{
	class FRAMWORK_API GenericJointComponent : public Component
	{
		DECL_OBJECT(GenericJointComponent)

	public:
		GenericJointComponent(GameObject* pGameObject);

		virtual void	Serialize(Serializer& sl, const char* pszLable = "GenericJointComponent")
		{
			ASSERT(false);
		}

		IPhysicsGenericJoint* GetPhysicsGenericJoint() {return m_pPhysicsGenericJoint;}

	private:
		IPhysicsGenericJoint*	m_pPhysicsGenericJoint;

	};

	class FRAMWORK_API FixJointComponent : public Component
	{
		DECL_OBJECT(FixJointComponent)

	public:
		FixJointComponent(GameObject* pGameObject);

		virtual void	Serialize(Serializer& sl, const char* pszLable = "FixJointComponent");

		IPhysicsJoint*	GetPhysicsFixJoint() {return m_pPhysicsFixJoint;}
		
	private:
		IPhysicsJoint* m_pPhysicsFixJoint;
	};

	
	class FRAMWORK_API HingJointComponent : public Component
	{
		DECL_OBJECT(HingJointComponent)

	public:
		HingJointComponent(GameObject* pGameObject);

		virtual void	Serialize(Serializer& sl, const char* pszLable = "HingJointComponent")
		{
			ASSERT(false);
		}
		
		IPhysicsHingeJoint*	GetPhysicsHingeJoint() {return m_pPhysicsHingeJoint;}

	private:
		IPhysicsHingeJoint*	m_pPhysicsHingeJoint;
	};

}

#endif