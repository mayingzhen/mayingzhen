#pragma once
#include "Node.h"

namespace ma
{
class WwiseBank;

class CWwiseEvent : public CNode
{
public:
	CWwiseEvent(WwiseBank* pWwiseBank);
	~CWwiseEvent(void);

	// ---------------------------------------------------------------------
	// Overwrite from CNode
	// ---------------------------------------------------------------------
public:
	DECLARE_NODE(CWwiseEvent, "FMODEvent");
	virtual int GetClassType() const{return CLASS_FMODEvent;}
	virtual bool IsReady();
	void Process();
	void Show(CCamera* pCamera, bool bCascade);
	void Reset();
	void SetCullTree(CCullTree* pCullTree);
	virtual void SetColor(const ColourValue& color, bool bCascade = false);
	virtual void SetAlpha(float fAlpha, bool bCascade = false);
	virtual void SetVisible(bool bVisible, bool bCascade = false);

	virtual void Play(uint32 nElapsedTime = 0, bool bCascade = false);
	virtual void Stop(bool bCascade = false);

	void SetSwitches(const char* pszSwithGroup,const char* pszSwith); 

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:
	bool Load(const char* pszEvent);
	void SetParam(const char* pszName, float fValue);
	bool IsPlaying() const;
private:
	RefPtr<WwiseBank> m_pWwiseBank;
	bool m_bOnLoadOver;
	bool m_bPlayed;
	bool m_bStopped;
	string m_strEvent;
	//FMOD::Studio::EventInstance* m_pFEventInstance;

	uint32	m_nGameObjectID;

	typedef map<string, float> MAP_STR_FLOAT;
	MAP_STR_FLOAT m_mapParam;
};

}