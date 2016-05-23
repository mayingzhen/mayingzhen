#include "WwiseBank.h"
#include "WwiseSystem.h"

#include "AK/SoundEngine/Common/AkTypes.h"
#include "AK/SoundEngine/Common/AkSoundEngine.h"

namespace ma
{


WwiseBank::WwiseBank(void)
{
	m_nBankID = AK_INVALID_AUX_ID;
	m_bOnLoadOver = false;
}

WwiseBank::~WwiseBank(void)
{
	if (m_nBankID != AK_INVALID_AUX_ID)
	{
		AK::SoundEngine::UnloadBank(m_nBankID,NULL);
		m_nBankID = AK_INVALID_AUX_ID;
	}
}

bool WwiseBank::Load( const char* pszFile )
{
	AKRESULT res = AK::SoundEngine::LoadBank(pszFile, AK_DEFAULT_POOL_ID, m_nBankID);
	if (res != AK_Success )
	{
		LogError( "Load Bank %s failed", pszFile);
		return false;
	}

	return true;
}

bool WwiseBank::InitRes()
{
	//this->SetState(LOADSTATE_INITED);

	return true;
}

RefPtr<WwiseBank> CreateWwiseBank(const char* pszFile)
{
	return GetWwiseBankManager()->CreateResource(pszFile);
}



ResourceSystem<WwiseBank>* g_pWwiseBankManger = NULL;
ResourceSystem<WwiseBank>* GetWwiseBankManager()
{
	return g_pWwiseBankManger;
}

}