#pragma once

namespace ma
{
	class D3D11Resource
	{
	public:
		D3D11Resource();

		virtual ~D3D11Resource();

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

