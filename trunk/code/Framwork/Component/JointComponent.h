#ifndef __JointComponent_H__
#define __JointComponent_H__

namespace ma
{
	class GenericJointComponent : public Component
	{
	public:
		GenericJointComponent(GameObject* pGameObject);

		IPhysicsGenericJoint* GetPhysicsGenericJoint() {return m_pPhysicsGenericJoint;}

	private:
		IPhysicsGenericJoint*	m_pPhysicsGenericJoint;

	};

	class FixJointComponent : public Component
	{
	public:
		FixJointComponent(GameObject* pGameObject);

		IPhysicsJoint*	GetPhysicsFixJoint() {return m_pPhysicsFixJoint;}
		
	private:
		IPhysicsJoint* m_pPhysicsFixJoint;
	};

	
	class HingJointComponent : public Component
	{
	public:
		HingJointComponent(GameObject* pGameObject);
		
		IPhysicsHingeJoint*	GetPhysicsHingeJoint() {return m_pPhysicsHingeJoint;}

	private:
		IPhysicsHingeJoint*	m_pPhysicsHingeJoint;
	};

}

#endif