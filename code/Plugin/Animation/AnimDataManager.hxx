#include "AnimDataManager.h"
//#include "Timer.h"

namespace ma
{

AnimDataManager::AnimDataManager(void)
{
}

AnimDataManager::~AnimDataManager(void)
{
}

RefPtr<Animation> AnimDataManager::Open( const char* pszFile, const char* pszSkeleton)
{
	string strSkaFile = pszFile;
	strSkaFile = StringUtil::replaceAll(strSkaFile, "\\", "/");
    strSkaFile = StringUtil::replaceAll(strSkaFile, "//", "/");
    StringUtil::toLowerCase(strSkaFile);
	uint32 uHash = Math::FastHash(strSkaFile.c_str(), strlen(strSkaFile.c_str()));
	MAP_HASH_RES::iterator iter = m_mapFactory.find(uHash);

	// not find
	if (iter == m_mapFactory.end())
	{
		RefPtr<Animation> res = this->CreateRes(strSkaFile.c_str(), pszSkeleton);// maybe NULL
		m_mapFactory[uHash][RES_INFO(strSkaFile.c_str(), pszSkeleton)] = res;
		return res;
	}

	MAP_STR_RES& mapStrRes = iter->second;
	MAP_STR_RES::iterator iterRes = mapStrRes.find(RES_INFO(strSkaFile.c_str(), pszSkeleton));

	// not find
	if (iterRes == mapStrRes.end())
	{
		RefPtr<Animation> res = this->CreateRes(strSkaFile.c_str(), pszSkeleton); // maybe NULL
		m_mapFactory[uHash][RES_INFO(strSkaFile.c_str(), pszSkeleton)] = res;
		return res;
	}

	//if (iterRes->second)
	//	iterRes->second->SetFreeTime(0);
	return iterRes->second;
}

void AnimDataManager::Clear( bool bRemoveAll )
{
    if (bRemoveAll)
    {
        m_mapFactory.clear();
        return;
    }

    for (MAP_HASH_RES::iterator iter = m_mapFactory.begin();iter != m_mapFactory.end();)
    {
        MAP_STR_RES& map1 = iter->second;
        for (MAP_STR_RES::iterator it1 = map1.begin();it1 != map1.end();)
        {
            Animation* pRes = it1->second.get();
            if (pRes == NULL || pRes->GetRefCount() == 1)
            {
                map1.erase(it1++);
            }
            else
            {
                ++it1;
            }
        }

        if (map1.empty())
        {
            m_mapFactory.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
}

// void AnimDataManager::FreeTick(bool bForceRelease, uint32 nResTime)
// {
//     uint32 nCurTime = GetTimer()->GetTime();
// 
//     for (MAP_HASH_RES::iterator iter = m_mapFactory.begin();iter != m_mapFactory.end();)
//     {
//         MAP_STR_RES& map1 = iter->second;
//         for (MAP_STR_RES::iterator it1 = map1.begin();it1 != map1.end();)
//         {
//             CAnimData* pRes = it1->second.get();
//             if (pRes == NULL)
//             {
//                 map1.erase(it1++);
//             }
//             else if (pRes->GetRefCount() == 1)
//             {
//                 if (bForceRelease)
//                 {
//                     map1.erase(it1++);
//                 }
//                 else
//                 {
//                     uint32 nFreeTime = pRes->GetFreeTime();
//                     if (nFreeTime == 0)
//                     {
//                         pRes->SetFreeTime(nCurTime);
//                         ++it1;
//                     }
//                     else if (nCurTime - nFreeTime > nResTime)
//                     {
//                         map1.erase(it1++);
//                     }
//                     else
//                     {
//                         ++it1;
//                     }
//                 }
//             }
//             else
//             {
//                 ++it1;
//             }
//         }
// 
//         if (map1.empty())
//         {
//             m_mapFactory.erase(iter++);
//         }
//         else
//         {
//             ++iter;
//         }
//     }
// }

AnimDataManager* g_pAnimDataManager = NULL;

AnimDataManager* GetAnimDataManager()
{
	return g_pAnimDataManager;
}
}