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

		virtual void					SetLayerCollisionMask(Uint8 nLayer,Uint8 nColLayer,bool bCollide) = 0;

		virtual Vector3					GetGravity() const = 0;

		virtual void					SetGravity(const Vector3& g) = 0;

 		//virtual IPhysicsObject*			CreatePhysicsObject(GameObject* pGameObj) = 0;

		virtual	IRigidBody*				CreateRigidBody(SceneNode* pGameObj) = 0;

		virtual IBoxCollisionShape*		CreateBoxCollisionShape(SceneNode* pGameObj) = 0;

		virtual ISphereCollisionShape*	CreateSphereCollisionShape(SceneNode* pGameObj) = 0;

		virtual ICapsuleCollisionShape* CreateCapsuleCollisionShape(SceneNode* pGameObj) = 0;

		virtual ICollisionMaterial*		CreateCollisionMaterial(SceneNode* pGameObj) = 0;

		virtual ICharaControll*			CreateCharaControll(SceneNode* pGameObj) = 0;

		virtual	IPhysicsGenericJoint*	CreatePhysicsGenericJoint(SceneNode* pGameObj) = 0;

		virtual	IPhysicsHingeJoint*		CreatePhysicsHingeJoint(SceneNode* pGameObj) = 0;

		//virtual void					DeletePhysicsObject(IPhysicsObject* pPhysicsObject) = 0;

		virtual	void					DeleteRigidBody(IRigidBody* pRigidBody) = 0;

		virtual void					DeleteBoxCollisionShape(IBoxCollisionShape*	pBox) = 0;

		virtual void					DeleteSphereCollisionShape(ISphereCollisionShape* pSphere) = 0;

		virtual	void					DeleteCapsuleCollisionShape(ICapsuleCollisionShape* pCapsule)  = 0;

		virtual void					DeleteCharaControll(ICharaControll* pCharcontrol) = 0;

		virtual	void					DeletePhysicsGenericJoint(IPhysicsGenericJoint* pJoint) = 0;

		virtual	void					DeletePhysicsHingeJoint(IPhysicsHingeJoint* pJoint) = 0;

		virtual SceneNode*				RayCastCloseGameObj(const Vector3& rayOrig, const Vector3& rayDir, int nCollLayer, Vector3& hitPosWS) = 0;

	};

	ENGINE_API void SetPhysicsSystem(IPhysicsSystem* pPhysicsSystem);

	ENGINE_API IPhysicsSystem* GetPhysicsSystem();
}


#endif // _IPhysicsSystem__H__
