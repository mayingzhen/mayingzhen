#pragma once
#include "Animation.h"


namespace ma
{

class AnimDataManager
{
public:
	AnimDataManager(void);
	~AnimDataManager(void);

	RefPtr<Animation> Open(const char* pszFile, const char* pszSkeleton,const char* pszRefSkeleton);
	void Clear(bool bRemoveAll);
    //void FreeTick(bool bForceRelease, uint32 nResTime);

private:
	RefPtr<Animation> CreateRes(const char* pszFile, const char* pszSkeleton,const char* pszRefSkeleton)
	{
		RefPtr<Animation> pAnimData = new Animation();
		pAnimData->Load(pszFile,pszSkeleton,pszRefSkeleton);
		return pAnimData;
	}

private:
    struct RES_INFO
    {
        string strSka;
        string strSke;
		string strRefSke;

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

			if (strRefSke<c.strRefSke)
				return true;
			else if (strRefSke>c.strRefSke)
				return false;
		
            return false;
        }
    };

	typedef map<RES_INFO, RefPtr<Animation> > MAP_STR_RES;
	typedef map<uint32, MAP_STR_RES> MAP_HASH_RES;
	MAP_HASH_RES m_mapFactory;
};

extern AnimDataManager* g_pAnimDataManager;
AnimDataManager* GetAnimDataManager();

}