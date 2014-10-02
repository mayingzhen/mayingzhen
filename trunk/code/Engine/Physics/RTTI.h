#ifndef  _IPhysicsSystem_RTTI__H__
#define  _IPhysicsSystem_RTTI__H__

namespace ma
{
#define RTTI_DECL(ClassType,ShowType) \
	Object* Create_##ShowType(void* arg) { return GetPhysicsSystem()->Create##ShowType((SceneNode*)arg);} \
	void	Delete_##ShowType(Object* pObje) {GetPhysicsSystem()->Delete##ShowType((ClassType*)pObje);}

	RTTI_DECL(IBoxCollisionShape,BoxCollisionShape);
	RTTI_DECL(ISphereCollisionShape,SphereCollisionShape);
	RTTI_DECL(ICapsuleCollisionShape,CapsuleCollisionShape);
	RTTI_DECL(IRigidBody,RigidBody);
	RTTI_DECL(ICharaControll,CharaControll);
	RTTI_DECL(IPhysicsGenericJoint,PhysicsGenericJoint);
	RTTI_DECL(IPhysicsHingeJoint,PhysicsHingeJoint);

#undef RTTI_DECL


	void IPhysicsSystemRTTIInit()
	{
		ICollisionShape::StaticInitClass();
		IPhysicsJoint::StaticInitClass();

#define RTTI_DECL(ClassType,ShowType) \
	ClassType::StaticInitClass(); \
	ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ShowType); \
	ObjectFactoryManager::GetInstance().RegisterObjectDeleteFactory(#ClassType,Delete_##ShowType);

		RTTI_DECL(IBoxCollisionShape,BoxCollisionShape);
		RTTI_DECL(ISphereCollisionShape,SphereCollisionShape);
		RTTI_DECL(ICapsuleCollisionShape,CapsuleCollisionShape);
		RTTI_DECL(IRigidBody,RigidBody);
		RTTI_DECL(ICharaControll,CharaControll);
		RTTI_DECL(IPhysicsGenericJoint,PhysicsGenericJoint);
		RTTI_DECL(IPhysicsHingeJoint,PhysicsHingeJoint);
#undef RTTI_DECL

	}

	void IPhysicsSystemRTTIShutdown()
	{

	}

}


#endif