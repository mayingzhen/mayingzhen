#pragma once

#include "AK/SoundEngine/Common/AkTypes.h"

namespace ma
{

class WwiseBank : public Resource
{
public:
	WwiseBank();
	~WwiseBank(void);

	// ---------------------------------------------------------------------
	// Inherite from Resource
	// ---------------------------------------------------------------------
	virtual bool Load(const char* pszFile);
	virtual bool InitRes();

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:

private:
	AkBankID m_nBankID; 
	bool m_bOnLoadOver;
};

RefPtr<WwiseBank> CreateWwiseBank(const char* pszFile);

extern ResourceSystem<WwiseBank>* g_pWwiseBankManger;
ResourceSystem<WwiseBank>* GetWwiseBankManager();

}