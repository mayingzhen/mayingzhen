#include "S3EngineCommon.h"
#include "WwiseEvent.h"
#include "WwiseSystem.h"
#include "Root.h"
namespace ma
{
IMPLEMENT_NODE(CWwiseEvent);
CWwiseEvent::CWwiseEvent(WwiseBank* pWwiseBank)
{
	m_pWwiseBank = pWwiseBank;
	m_bOnLoadOver = false;
	m_bPlayed = false;
	m_bStopped = false;
	//m_pFEventInstance = NULL;

	m_nGameObjectID = AK_INVALID_GAME_OBJECT;
}

CWwiseEvent::~CWwiseEvent(void)
{
	m_pWwiseBank = NULL;
// 	if (m_pFEventInstance != NULL)
// 	{
// 		m_pFEventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
// 		m_pFEventInstance->release();
// 		m_pFEventInstance = NULL;
// 	}
}

void CWwiseEvent::SetSwitches(const char* pszSwithGroup,const char* pszSwith)
{
	AK::SoundEngine::SetSwitch(pszSwithGroup,pszSwith,m_nGameObjectID);
}

// ---------------------------------------------------------------------
// Overwrite from CNode
// ---------------------------------------------------------------------
bool CWwiseEvent::IsReady()
{
	if (m_bOnLoadOver)
		return true;

	if (!m_pWwiseBank->IsReady())
		return false;
	
	m_bOnLoadOver = true;

	if (m_bPlayed)
	{
		FMOD::Studio::EventDescription* event = NULL;
		FMOD_RESULT hr = g_pFSystem->getEvent(m_strEvent.c_str(), &event);
		if (hr != FMOD_OK)
		{
			LogError("%s", FMOD_ErrorString(hr));
			return false;
		}

		if (m_pFEventInstance)
		{
			m_pFEventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
			m_pFEventInstance->release();
			m_pFEventInstance = NULL;
		}
		hr = event->createInstance(&m_pFEventInstance);
		if (hr != FMOD_OK)
		{
			LogError("%s", FMOD_ErrorString(hr));
			return false;
		}

		// 3d
		bool b3D = false;
		hr = event->is3D(&b3D);
		if (hr != FMOD_OK)
		{
			LogError("%s", FMOD_ErrorString(hr));
			return false;
		}

		if(b3D)
		{
			Matrix3 mat3;
			this->GetWorldMatrix().extract3x3Matrix(mat3);
			Vector3 vForward = mat3*Vector3::UNIT_Z;
			vForward.normalise();
			Vector3 vUp = mat3*Vector3::UNIT_Y;
			vUp.normalise();

			FMOD_3D_ATTRIBUTES att;
			att.position = *(FMOD_VECTOR*)&this->GetWorldPos();
			att.forward = *(FMOD_VECTOR*)&vForward;
			att.up = *(FMOD_VECTOR*)&vUp;
			att.velocity = *(FMOD_VECTOR*)&Vector3::ZERO;
			VERIFY(m_pFEventInstance->set3DAttributes(&att) == FMOD_OK);
		}

		{
			for (MAP_STR_FLOAT::iterator iter = m_mapParam.begin();iter != m_mapParam.end();++iter)
			{
				VERIFY(m_pFEventInstance->setParameterValue(iter->first.c_str(), iter->second) == FMOD_OK);
			}

			hr = m_pFEventInstance->start();
			if (hr != FMOD_OK)
			{
				LogError("%s", FMOD_ErrorString(hr));
				return false;
			}

			if (m_bStopped)
			{
				m_pFEventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
			}
		}
	}
	
	return true;
}

void CWwiseEvent::Process()
{
	this->IsReady();

	if (CNode::BeginMatrix())
	{
		CNode::CalcMatrix();

		if (m_pFEventInstance != NULL)
		{
			Matrix3 mat3;
			this->GetWorldMatrix().extract3x3Matrix(mat3);
			Vector3 vForward = mat3*Vector3::UNIT_Z;
			vForward.normalise();
			Vector3 vUp = mat3*Vector3::UNIT_Y;
			vUp.normalise();

			FMOD_3D_ATTRIBUTES att;
			att.position = *(FMOD_VECTOR*)&this->GetWorldPos();
			att.forward = *(FMOD_VECTOR*)&vForward;
			att.up = *(FMOD_VECTOR*)&vUp;
			att.velocity = *(FMOD_VECTOR*)&Vector3::ZERO;
			VERIFY(m_pFEventInstance->set3DAttributes(&att) == FMOD_OK);
		}
		CNode::EndMatrix();
	}
	CNode::Process();

	if (m_pFEventInstance != NULL)
	{
		FMOD_STUDIO_PLAYBACK_STATE state;
		if(m_pFEventInstance->getPlaybackState(&state) == FMOD_OK)
		{
			if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
			{
				if (this->GetParent() != NULL)
				{
					g_lstAutoDeleteNodes.push_back(this);
					this->GetParent()->RemoveChild(this);
					return;
				}
			}
		}
	}
}

void CWwiseEvent::Show( CCamera* pCamera, bool bCascade )
{
	CNode::Show(pCamera, bCascade);
}

void CWwiseEvent::Reset()
{
	CNode::Reset();
}

void CWwiseEvent::SetCullTree( CCullTree* pCullTree )
{
	CNode::SetCullTree(pCullTree);
}

void CWwiseEvent::SetColor( const ColourValue& color, bool bCascade /*= false*/ )
{
	CNode::SetColor(color, bCascade);
}

void CWwiseEvent::SetAlpha( float fAlpha, bool bCascade /*= false*/ )
{
	CNode::SetAlpha(fAlpha);
}

void CWwiseEvent::SetVisible( bool bVisible, bool bCascade /*= false*/ )
{
	CNode::SetVisible(bVisible, bCascade);
}

void CWwiseEvent::Play( uint32 nElapsedTime /*= 0*/, bool bCascade /*= false*/ )
{
	CNode::Play(nElapsedTime, bCascade);

	m_bPlayed = true;
	if (!m_bOnLoadOver)
	{
		return;
	}

	m_bOnLoadOver = false;
	this->IsReady();
}

void CWwiseEvent::Stop( bool bCascade /*= false*/ )
{
	CNode::Stop(bCascade);

	m_bStopped = true;
	if (m_pFEventInstance)
	{
		VERIFY(m_pFEventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT) == FMOD_OK);
	}
}

// ---------------------------------------------------------------------
// Self
// ---------------------------------------------------------------------
bool CWwiseEvent::Load( const char* pszEvent )
{
	if (pszEvent == NULL)
	{
		LogError("Failed to Load CFMODEvent as pszEvent is NULL!");
		return false;
	}

	m_strEvent = pszEvent;
	return true;
}

void CWwiseEvent::SetParam( const char* pszName, float fValue )
{
	m_mapParam[pszName] = fValue;

	if (m_pFEventInstance != NULL)
	{
		VERIFY(m_pFEventInstance->setParameterValue(pszName, fValue) == FMOD_OK);
	}
}

}