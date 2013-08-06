#ifndef  _IPhysicsScene__H__
#define  _IPhysicsScene__H__

namespace ma
{
	class GameObject;

	class ENGINE_API IPhysicsScene
	{
	public:
		virtual bool					Start()	= 0;

		virtual void					Stop() = 0;

		virtual void					SetLayerCollisionMask(Uint8 nLayer,Uint8 nColLayer,bool bCollide) = 0;

		virtual Vector3					GetGravity() const = 0;

		virtual void					SetGravity(const Vector3& g) = 0;

		virtual void					BeginSimulation(float fDeltaTime) = 0;

		virtual void					EndSimulation() = 0;

		virtual	IPhysicsObject*			CreatePhysicsObject(GameObject* pGameObj) = 0;

		virtual	IPhysicsGenericJoint*	CreatePhysicsGenericJoint(IPhysicsObject* pPhyObjA, IPhysicsObject* pPhyObjB) = 0;

		virtual	IPhysicsHingeJoint*		CreatePhysicsHingeJoint(IPhysicsObject* pPhyObjA, IPhysicsObject* pPhyObjB) = 0;

		virtual GameObject*				RayCastCloseGameObj(const Vector3& rayOrig, const Vector3& rayDir, int nCollLayer, Vector3& hitPosWS) = 0;
	};
}

#endif

