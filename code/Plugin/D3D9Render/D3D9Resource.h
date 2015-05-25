#ifndef  _D3D9Resource__H__
#define  _D3D9Resource__H__

namespace ma
{
	class D3D9Resource
	{
	public:
		D3D9Resource();

		virtual ~D3D9Resource();

		// Called before the Direct3D device is going to be destroyed.
		virtual void NotifyOnDeviceDestroy() {}

		// Called immediately after the Direct3D device has entered a lost state.
		// This is the place to release non-managed resources.
		virtual void NotifyOnDeviceLost() {}

		// Called immediately after the Direct3D device has been reset.
		// This is the place to create non-managed resources.
		virtual void NotifyOnDeviceReset() {}
	};

}

#endif