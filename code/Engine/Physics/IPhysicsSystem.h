#ifndef  _IPhysicsSystem__H__
#define  _IPhysicsSystem__H__

namespace ma
{
	class IPhysicsObject;
	class IPhysicsGenericJoint;
	class IPhysicsHingeJoint;
	class IRigidBody;
	class SceneNode;

	class ENGINE_API IPhysicsSystem 
	{
	public:
		virtual	void					Init() = 0;

		virtual void					ShoutDown() = 0;

		virtual	void					Start() = 0;

		virtual void					Stop() = 0;

		virtual void					BeginUpdate() = 0;

		virtual	void					EndUpdate()  = 0;

		virtual void					DebugRender() = 0;

		virtual void					SetLayerCollisionMask(uint8 nLayer,uint8 nColLayer,bool bCollide) = 0;

		virtual Vector3					GetGravity() const = 0;

		virtual void					SetGravity(const Vector3& g) = 0;

		virtual	IRigidBody*				CreateRigidBody() = 0;

		virtual IBoxCollisionShape*		CreateBoxCollisionShape() = 0;

		virtual ISphereCollisionShape*	CreateSphereCollisionShape() = 0;

		virtual ICapsuleCollisionShape* CreateCapsuleCollisionShape() = 0;

		virtual ICollisionMaterial*		CreateCollisionMaterial() = 0;

		virtual ICharaControll*			CreateCharaControll() = 0;

		virtual	IPhysicsGenericJoint*	CreatePhysicsGenericJoint() = 0;

		virtual	IPhysicsHingeJoint*		CreatePhysicsHingeJoint() = 0;

		virtual SceneNode*				RayCastCloseGameObj(const Vector3& rayOrig, const Vector3& rayDir, int nCollLayer, Vector3& hitPosWS) = 0;

	};

	ENGINE_API void SetPhysicsSystem(IPhysicsSystem* pPhysicsSystem);

	ENGINE_API IPhysicsSystem* GetPhysicsSystem();
}


#endif // _IPhysicsSystem__H__
