#ifndef  _IPhysicsSystem__H__
#define  _IPhysicsSystem__H__

namespace ma
{
	class IPhysicsObject;
	class IPhysicsGenericJoint;
	class IPhysicsHingeJoint;
	class GameObject;

	class ENGINE_API IPhysicsSystem 
	{
	public:
		virtual	void					Init() = 0;

		virtual void					ShoutDown() = 0;

		virtual	void					Start() = 0;

		virtual void					Stop() = 0;

		virtual void					Update() = 0;

		virtual void					DebugRender() = 0;

		virtual void					SetLayerCollisionMask(Uint8 nLayer,Uint8 nColLayer,bool bCollide) = 0;

		virtual Vector3					GetGravity() const = 0;

		virtual void					SetGravity(const Vector3& g) = 0;

 		virtual IPhysicsObject*			CreatePhysicsObject(GameObject* pGameObj) = 0;

		virtual void					DeletePhysicsObject(IPhysicsObject* pPhysicsObject) = 0;

		virtual	IPhysicsGenericJoint*	CreatePhysicsGenericJoint(IPhysicsObject* pPhyObjA, IPhysicsObject* pPhyObjB) = 0;

		virtual	IPhysicsHingeJoint*		CreatePhysicsHingeJoint(IPhysicsObject* pPhyObjA, IPhysicsObject* pPhyObjB) = 0;

		virtual GameObject*				RayCastCloseGameObj(const Vector3& rayOrig, const Vector3& rayDir, int nCollLayer, Vector3& hitPosWS) = 0;

	};

	ENGINE_API void SetPhysicsSystem(IPhysicsSystem* pPhysicsSystem);

	ENGINE_API IPhysicsSystem* GetPhysicsSystem();
}


#endif // _IPhysicsSystem__H__
