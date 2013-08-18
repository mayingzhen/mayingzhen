#ifndef __JointComponent_H__
#define __JointComponent_H__

namespace ma
{
	class ENTITYSYSTEM_API GenericJointComponent : public Component
	{
		DECL_OBJECT(GenericJointComponent)

	public:
		GenericJointComponent(GameObject* pGameObject);

		~GenericJointComponent();

		virtual void	Serialize(Serializer& sl, const char* pszLable = "GenericJointComponent")
		{
			ASSERT(false);
		}

		IPhysicsGenericJoint* GetPhysicsGenericJoint() {return m_pPhysicsGenericJoint;}

	private:
		IPhysicsGenericJoint*	m_pPhysicsGenericJoint;

	};

	class ENTITYSYSTEM_API FixJointComponent : public Component
	{
		DECL_OBJECT(FixJointComponent)

	public:
		FixJointComponent(GameObject* pGameObject);

		~FixJointComponent();

		virtual void	Serialize(Serializer& sl, const char* pszLable = "FixJointComponent");

		IPhysicsJoint*	GetPhysicsFixJoint() {return m_pPhysicsFixJoint;}
		
	private:
		IPhysicsJoint* m_pPhysicsFixJoint;
	};

	
	class ENTITYSYSTEM_API HingJointComponent : public Component
	{
		DECL_OBJECT(HingJointComponent)

	public:
		HingJointComponent(GameObject* pGameObject);

		~HingJointComponent();

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