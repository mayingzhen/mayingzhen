#include "Sample.h"

namespace ma
{
	Sample::Sample(const char* pszName)
	{
		m_pScene = GetRenderSystem()->GetView(0)->GetScene();
		m_pCamera = GetRenderSystem()->GetView(0)->GetCamera();
		//m_pScene = new Scene(pszName);

// 		GetInput()->AddKeyListener(this);
// 		GetInput()->AddMouseListener(this);
// 		GetInput()->AddTouchListener(this);
	}

	Sample::~Sample()
	{
// 		GetInput()->RemoveKeyListener(this);
// 		GetInput()->RemoveMouseListener(this);
// 		GetInput()->RemoveTouchListener(this);	

		//m_pScene = NULL;
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

