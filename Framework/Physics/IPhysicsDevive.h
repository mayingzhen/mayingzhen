#ifndef  _IPhysicsDevice__H__
#define  _IPhysicsDevice__H__

namespace ma
{
	class IPhysicsScene;
	class IPhysicsObject;
	class IRigidBody;
	class IBoxCollisionShape;
	class ISphereCollisionShape;
	class ICharacterController;

	class FRAMEWORK_API IPhysicsDevice
	{
	public:
		virtual IPhysicsScene*			CreatePhysicsScene() = 0;
	
		virtual IPhysicsObject*			CreatePhysicsObject() = 0;

		virtual IRigidBody*				CreateRigidBody()  = 0;

		virtual IBoxCollisionShape*		CreateBoxCollisionShape() = 0;

		virtual ISphereCollisionShape*	CreateSphereCollisionShape() = 0;

		virtual ICharacterController*	CreateCharacterController() = 0;
	};

	FRAMEWORK_API void SetPhysicsDevice(IPhysicsDevice* pPhysicsDevice);

	FRAMEWORK_API IPhysicsDevice* GetPhysicsDevice();
}


#endif
