#pragma once
#include "Animation.h"


namespace ma
{

class AnimDataManager
{
public:
	AnimDataManager(void);
	~AnimDataManager(void);

	RefPtr<Animation> Open(const char* pszFile, const char* pszSkeleton, const RES_CALL_BACK& call_back = nullptr);
	void Clear(bool bRemoveAll);
    //void FreeTick(bool bForceRelease, uint32_t nResTime);

private:
	RefPtr<Animation> CreateRes(const char* pszFile, const char* pszSkeleton,const RES_CALL_BACK& call_back = nullptr)
	{
		RefPtr<Animation> pAnimData = new Animation();
		pAnimData->AddCallBack(call_back);
		pAnimData->Load(pszFile,pszSkeleton);
		return pAnimData;
	}

private:
    struct RES_INFO
    {
        string strSka;
        string strSke;

        RES_INFO(){}
        RES_INFO(const string& _strSka, const string& _strSke):strSka(_strSka),strSke(_strSke){}
        bool operator < (const RES_INFO& c) const
        {
            if (strSka<c.strSka)
                return true;
            else if(strSka>c.strSka)
                return false;

			if (strSke<c.strSke)
				return true;
			else if(strSke>c.strSke)
				return false;
		
            return false;
        }
    };

	typedef map<RES_INFO, RefPtr<Animation> > MAP_STR_RES;
	typedef map<uint32_t, MAP_STR_RES> MAP_HASH_RES;
	MAP_HASH_RES m_mapFactory;
};

extern AnimDataManager* g_pAnimDataManager;
AnimDataManager* GetAnimDataManager();

}