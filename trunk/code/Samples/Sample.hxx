#include "Samples/Sample.h"

namespace ma
{

	CameraController* Sample::GetCameraControll()
	{
		return GetSampleBrowser()->GetCameraController();
	}

	Camera*	Sample::GetCamera()
	{
		return GetEntitySystem()->GetCamera();
	}

}

