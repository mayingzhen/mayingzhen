#include "Animation/Skeleton.h"

namespace ma
{


	void Skeleton::Serialize(SerializeListener& sl,const char* pszLabel)
	{
		//StackMemoryMark memMarker(GetDefaultStackAllocator());

		ExpSkeletonHeader skelHeader;
		ExpSkeletonData skelData;
		sl.Serialize(skelHeader,"Header");
		sl.PushVersion(skelHeader.m_nVersion);
		sl.Serialize(skelData,"Skeleton");
		sl.PopVersion();

	
		xmUint nBoneNum = skelData.m_nBoneNum;

		std::vector<xmNodeTransform> arrNodeOS;
		arrNodeOS.resize(nBoneNum);
		for (xmUint nBoneCnt = 0; nBoneCnt < nBoneNum; ++nBoneCnt)
		{
			arrNodeOS[nBoneCnt].m_vPos = skelData.m_arrPosOS[nBoneCnt];
			arrNodeOS[nBoneCnt].m_qRot = skelData.m_arrRotOS[nBoneCnt];
			m_arrBoneName[nBoneCnt] = skelData.m_arrBoneName[nBoneCnt];
			m_arrParentInd[nBoneCnt] = skelData.m_arrParentIndice[nBoneCnt];
		}

		m_refPose.InitObjectSpace(arrNodeOS, m_arrParentInd);

		m_arrRefPosePS.resize(nBoneNum);
		m_arrRefPoseOS.resize(nBoneNum);
		m_arrRefPoseOSInv.resize(nBoneNum);

		for (UINT uBoneCunt = 0; uBoneCunt < nBoneNum; ++uBoneCunt)
		{
			maMatrixFromTransform( &m_arrRefPoseOS[uBoneCunt], &m_refPose.GetTransformOS(uBoneCunt) );
			maMatrixFromTransform( &m_arrRefPosePS[uBoneCunt], &m_refPose.GetTransformPS(uBoneCunt) );
			D3DXMatrixInverse( &m_arrRefPoseOSInv[uBoneCunt], NULL, &m_arrRefPoseOS[uBoneCunt] );
		}
		//pSkel->Init(pSkelData->m_arrBoneName
		//	,arrParentIndex
		//	,pSkelData->m_arrScaleOS
		//	,pSkelData->m_arrRotOS
		//	,pSkelData->m_arrPosOS
		//	,pSkelData->m_nGlobalSkeletonID
		//	,pSkelData->m_arrLODBoneMask);

// 		for (xmUint nSocketCnt = 0; nSocketCnt < pSkelData->m_arrSocket.size(); ++nSocketCnt)
// 		{
// 			const S3AExpSocketData* pSocketData = &pSkelData->m_arrSocket[nSocketCnt];
// 			pSkel->AddSocket(pSocketData->m_strBoneName.c_str(),pSocketData->m_nBoneID,pSocketData->m_matTransformBS);
// 		}
// 
// 		pSkel->NotifySocketUpdate();

		//pSkel = reinterpret_cast<Skeleton*>(ExpDataConverter::CreateSkeleton(&skelData,skelHeader.m_nVersion));


// 		GlobalConfig* pCfg = reinterpret_cast<GlobalConfig*>(GetGlobalConfig());
// 		if (pCfg->m_bAutoSocketFromBoneEnable)
// 		{
// 			ArrayAlloc<const char*> arrSockePrefix(GetDefaultStackAllocator());
// 			if (pCfg->m_arrSocketFromBoneNamePrefix.size() < 512)
// 			{
// 				for (xmUint nPCnt = 0; nPCnt < pCfg->m_arrSocketFromBoneNamePrefix.size(); ++nPCnt)
// 				{
// 					arrSockePrefix.push_back(pCfg->m_arrSocketFromBoneNamePrefix[nPCnt].c_str());
// 				}
// 				if (arrSockePrefix.size() > 0)
// 				{
// 					SkelUtil::BuildSocketFromBone(pSkel,&arrSockePrefix[0],arrSockePrefix.size());
// 				}
// 
// 			}else{
// 				LogError(_ERR_INVALID_CALL,"Fail to auto build socket from bone : number of bone prefix is larger then 512\n");
// 			}
// 		}

		//#pragma message( __FILE__ "(" STRING(__LINE__) ") : TODO Remove UpdateSkeletonGUID on loading")


		//return pSkel;
	}

}