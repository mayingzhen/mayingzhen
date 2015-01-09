#include "Sample.h"

namespace ma
{
	Sample::Sample(const char* pszName)
	{
		m_pScene = GetRenderSystem()->GetScene();
		m_pCamera = m_pScene->GetCamera();
	}

	Sample::~Sample()
	{
	}

	CameraController* Sample::GetCameraControll()
	{
		return GetSampleBrowser()->GetCameraController();
	}

	Camera*	Sample::GetCamera()
	{
		return m_pCamera.get();
	}

}

