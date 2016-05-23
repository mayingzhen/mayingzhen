#pragma once

#include "WwiseListener.h"

namespace ma
{
class WwiseListener;

class WwiseSystem
{
public:
	WwiseSystem(void);

	~WwiseSystem(void);

	bool Init(HWND hwnd, const char* pszSoundBashPath,const char* pszLanguage);

	void Shoutdown();

	void Update();

	void SetState(const char* pszStateGroup,const char* pszState);
	
	void SetRTPValue(const char* pszRtpcName,float fValue);

	void StopAll();

	RefPtr<WwiseListener> GetDefaultListener() const;

private:
	RefPtr<WwiseListener> m_pDefaultListener;
};

extern WwiseSystem* g_pWwiseSystem;
WwiseSystem* GetWwiseSystem();

}