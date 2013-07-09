#ifndef  _IPhysicsDevice__H__
#define  _IPhysicsDevice__H__

namespace ma
{
	class IPhysicsScene;

	class ENGINE_API IPhysicsDevice /*: public IDevice*/
	{
	public:
		virtual IPhysicsScene*			CreatePhysicsScene() = 0;

		virtual void					DestoryPhysicsScene(IPhysicsScene* pPhysicsScene) = 0;
	};

	ENGINE_API void SetPhysicsDevice(IPhysicsDevice* pPhysicsDevice);

	ENGINE_API IPhysicsDevice* GetPhysicsDevice();
}


#endif
