#ifndef  _IPhysicsDevice__H__
#define  _IPhysicsDevice__H__

namespace ma
{
	class IPhysicsScene;
	class IPhysicsObject;

	class COMMON_API IPhysicsDevice
	{

	public:
		virtual IPhysicsScene* CreatePhysicsScene() = 0;
	
		virtual IPhysicsObject* CreatePhysicsObject() = 0;
	};

	COMMON_API void SetPhysicsDevice(IPhysicsDevice* pPhysicsDevice);

	COMMON_API IPhysicsDevice* GetPhysicsDevice();
}


#endif