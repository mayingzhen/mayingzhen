#include "WwiseListener.h"

#include "AK/SoundEngine/Common/AkTypes.h"
#include "Ak/SoundEngine/Common/AkSoundEngine.h"

namespace ma
{

WwiseListener::WwiseListener(int nIndex)
{
	m_nIndex = nIndex;
	m_bAutoUpdate = true;
}

WwiseListener::~WwiseListener(void)
{
}

void WwiseListener::Update()
{
// 	if (CNode::BeginMatrix())
// 	{
// 		CNode::CalcMatrix();
// 		if (m_bAutoUpdate)
// 		{
// 			Matrix3 mat3;
// 			this->GetWorldMatrix().extract3x3Matrix(mat3);
// 			Vector3 vForward = mat3*Vector3::UNIT_Z;
// 			vForward.normalise();
// 			Vector3 vUp = mat3*Vector3::UNIT_Y;
// 			vUp.normalise();
// 
// 			this->ManualUpdate(this->GetWorldPos(), vForward, vUp);
// 		}
// 		CNode::EndMatrix();
// 	}
// 	CNode::Process();
}

void WwiseListener::ManualUpdate(const Vector3& vPos, const Vector3& vForward, const Vector3& vUp)
{
	AkListenerPosition att;
	att.Position = *(AkVector*)&vPos;
	att.OrientationFront = *(AkVector*)&vForward;
	att.OrientationTop = *(AkVector*)&vUp;
	AKRESULT hr = AK::SoundEngine::SetListenerPosition(att,m_nIndex);
	ASSERT(hr == AK_Success);
	if (hr != AK_Success)
	{
		LogError("WwiseListener::ManualUpdate");
	}
}

}