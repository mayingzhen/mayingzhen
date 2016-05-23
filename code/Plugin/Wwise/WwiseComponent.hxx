#include "WwiseComponent.h"
#include "WwiseSystem.h"

#include "AK/SoundEngine/Common/AkTypes.h"
#include "AK/SoundEngine/Common/AkSoundEngine.h"

namespace ma
{

WwiseComponent::WwiseComponent(const char* pszName)
{
	m_strGameObjectName = pszName ? pszName : "";

	AKRESULT hr = AK::SoundEngine::RegisterGameObj((AkGameObjectID)this,m_strGameObjectName.c_str());
	ASSERT(hr == AK_Success);

	m_bAutoDestroy = false;
	m_bFlaggedForDestroy = false;
	m_nNumActiveEvents = 0;
}

WwiseComponent::~WwiseComponent(void)
{
	Stop(true);

	AKRESULT hr = AK::SoundEngine::UnregisterGameObj((AkGameObjectID)this);
	ASSERT(hr == AK_Success);
}

void WwiseComponent::Update()
{
// 	if (CNode::BeginMatrix())
// 	{
// 		CNode::CalcMatrix();
// 	
// 		Matrix3 mat3;
// 		this->GetWorldMatrix().extract3x3Matrix(mat3);
// 		Vector3 vForward = mat3*Vector3::UNIT_Z;
// 		vForward.normalise();
// 		Vector3 vUp = mat3*Vector3::UNIT_Y;
// 		vUp.normalise();
// 
// 		AkSoundPosition soundPos;
// 		soundPos.Position = *(AkVector*)&this->GetWorldPos();
// 		soundPos.Orientation.X = 0;
// 		soundPos.Orientation.Y = 0;
// 		soundPos.Orientation.Z = 1;
// 		AKRESULT res = AK::SoundEngine::SetPosition((AkGameObjectID)this, soundPos);
// 		ASSERT(res == AK_Success);
// 	
// 		CNode::EndMatrix();
// 	}
// 	CNode::Process();

// 	if(m_bAutoDestroy && m_bFlaggedForDestroy)
// 	{
// 		if (m_nNumActiveEvents <= 0)
// 		{
// 			g_lstAutoDeleteNodes.push_back(this);
// 			this->GetParent()->RemoveChild(this);
// 			return;
// 		}
// 		else
// 		{
// 			m_bFlaggedForDestroy = false;
// 		}
// 	}
}

void WwiseComponent::SetSwitches(const char* pszSwithGroup,const char* pszSwith)
{
	AKRESULT hr = AK::SoundEngine::SetSwitch(pszSwithGroup,pszSwith,(AkGameObjectID)this);
	ASSERT(hr == AK_Success);
}

static void AkEndCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
	WwiseComponent* pObj = (WwiseComponent*)in_pCallbackInfo->pCookie;
	if(pObj)
	{
		pObj->m_nNumActiveEvents = AtomicDecrement(&pObj->m_nNumActiveEvents);
		if( pObj->m_nNumActiveEvents == 0 )
		{
			pObj->m_bFlaggedForDestroy = true;
		}
	}
}

void WwiseComponent::PostEvent(const char* pszEvent)
{
	if (m_bAutoDestroy )
	{
		m_nPlayingID = AK::SoundEngine::PostEvent(pszEvent,(AkGameObjectID)this,AK_EndOfEvent, &AkEndCallback, this);
		ASSERT(m_nPlayingID != AK_INVALID_PLAYING_ID);
		if (m_nPlayingID != AK_INVALID_PLAYING_ID)
		{
			m_nNumActiveEvents = AtomicIncrement(&m_nNumActiveEvents);
		}
	}
	else
	{
		m_nPlayingID = AK::SoundEngine::PostEvent(pszEvent,(AkGameObjectID)this);
		ASSERT(m_nPlayingID != AK_INVALID_PLAYING_ID);
	}
}

void WwiseComponent::SetRTPValue(const char* pszRtpcName,float fValue)
{
	AKRESULT hr = AK::SoundEngine::SetRTPCValue(pszRtpcName,fValue,(AkGameObjectID)this);
	ASSERT(hr == AK_Success);
}

void WwiseComponent::Stop( bool bCascade /*= false*/ )
{
//	CNode::Stop(bCascade);

	//AK::SoundEngine::StopAll(m_nGameObjectID);
	AK::SoundEngine::StopPlayingID(m_nPlayingID);
}

RefPtr<WwiseComponent> CreateWwiseComponent(const char* pszName)
{
	RefPtr<WwiseComponent> pWwiseNode = new WwiseComponent(pszName);
	return pWwiseNode;
}

}