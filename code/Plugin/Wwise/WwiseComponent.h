#pragma once

namespace ma
{
class WwiseBank;

class WwiseComponent : public Component
{
public:
	WwiseComponent(const char* pszName);
	~WwiseComponent(void);

	virtual	void Update();

	virtual void Stop(bool bCascade = false);

	void SetAutoDestroy(bool b) {m_bAutoDestroy = b;}
	bool GetAutoDestroy() const {return m_bAutoDestroy;}

	void SetSwitches(const char* pszSwithGroup,const char* pszSwith); 

	void PostEvent(const char* pszEvent);
	
	void SetRTPValue(const char* pszRtpcName,float fValue);
	
private:
	string		m_strGameObjectName;

	uint32		m_nPlayingID;

	bool		m_bAutoDestroy;

public:
	volatile bool	m_bFlaggedForDestroy;
	volatile int	m_nNumActiveEvents;
};

RefPtr<WwiseComponent> CreateWwiseComponent(const char* pszName);

}