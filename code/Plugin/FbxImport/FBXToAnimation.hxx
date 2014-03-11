#include "FBXToAnimation.h"
#include "FbxImportUtil.h"

namespace ma
{
	void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode,FbxAnimStack* pAnimStack,std::map<std::string,TrackData>& boneTrack)
	{
		int lModelCount;
		FbxString lOutputString;

		lOutputString = "     Node Name: ";
		lOutputString += pNode->GetName();
		lOutputString += "\n\n";
		FBXSDK_printf(lOutputString);

		const char* pszBoneName = pNode->GetName();

		TrackData trackData;
	
		if ( pNode->GetSkeleton() )
		{
			FbxTimeSpan timeSpan = pAnimStack->GetLocalTimeSpan();
			FbxTime start = timeSpan.GetStart();
			FbxTime stop = timeSpan.GetStop();
			FbxTime duration = timeSpan.GetDuration();
			FbxLongLong frame = duration.GetFrameCount();
			FbxTime step;
			step.SetTime(0,0,0,1);
			int i = 0;
			while(start < stop)
			{
				start += step;

				FbxAMatrix mat = pNode->EvaluateLocalTransform(start);
				FbxVector4 pTranslation = mat.GetT();
				FbxQuaternion pRotation = mat.GetQ();
				FbxVector4 pScaling = mat.GetS();

				FbxVector4 euler = mat.GetR();

				trackData.posTrack.m_arrFrame.push_back(i);
				trackData.RotTrack.m_arrFrame.push_back(i);
				trackData.scaleTrack.m_arrFrame.push_back(i);

				trackData.posTrack.m_arrValue.push_back( ToMaUnit(pTranslation) );
				trackData.RotTrack.m_arrValue.push_back( ToMaUnit(pRotation) );
				trackData.scaleTrack.m_arrValue.push_back( ToMaUnit(pScaling) );

				++i;
			}

			boneTrack[pszBoneName] = trackData;	
		}

		for(lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
		{
			DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount),pAnimStack,boneTrack);
		}
	}

	void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode,Animation& skaData,const Skeleton& skeData)
	{
		int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
		FbxString lOutputString;

		lOutputString = "Animation stack contains ";
		lOutputString += nbAnimLayers;
		lOutputString += " Animation Layer(s)\n";
		FBXSDK_printf(lOutputString);

		std::map<std::string,TrackData> boneTrack;

		//ASSERT(nbAnimLayers == 1);
		for (int l = 0; l < nbAnimLayers; l++)
		{
			FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);

			lOutputString = "AnimLayer ";
			lOutputString += l;
			lOutputString += "\n";
			FBXSDK_printf(lOutputString);

		
			DisplayAnimation(lAnimLayer, pNode, pAnimStack, boneTrack);

			break;
		}
		
		ASSERT(boneTrack.size() == skeData.m_arrBoneName.size());
		UINT nBoneNum = boneTrack.size();
		skaData.m_arrTransfTrackName.resize(nBoneNum);
		skaData.m_arrPosTrack.resize(nBoneNum);
		skaData.m_arrRotTrack.resize(nBoneNum);
		skaData.m_arrScaleTrack.resize(nBoneNum);

 		for (UINT i = 0; i < nBoneNum; ++i)
		{
			std::string boneName = skeData.m_arrBoneName[i];
			std::map<std::string,TrackData>::iterator it = boneTrack.find( boneName.c_str() );
			ASSERT(it != boneTrack.end());
			if (it == boneTrack.end())
				continue;

			skaData.m_arrTransfTrackName[i] = skeData.m_arrBoneName[i];
			skaData.m_arrPosTrack[i] = it->second.posTrack;
			skaData.m_arrRotTrack[i] = it->second.RotTrack;
			skaData.m_arrScaleTrack[i] = it->second.scaleTrack;
			int nFrame = it->second.posTrack.m_arrFrame.size();
			skaData.m_nFrameNumber = skaData.m_nFrameNumber < nFrame ? nFrame : skaData.m_nFrameNumber;
		}
	}

	bool LoadAnimationData(const char* pFileName,const Skeleton& skeData,const char* pOutSkaFile)
	{
		if (pFileName == NULL)
			return false;

		FbxScene* pFbxScene = GetFbxScene(pFileName);
		if (pFbxScene == NULL)
			return false;

		Animation skaData;

		int nAnimStackCount = pFbxScene->GetSrcObjectCount<FbxAnimStack>();
		ASSERT(nAnimStackCount == 1);
		for (int i = 0; i < nAnimStackCount; i++)
		{
			FbxAnimStack* lAnimStack = pFbxScene->GetSrcObject<FbxAnimStack>(i);

			FbxString lOutputString = "Animation Stack Name: ";
			lOutputString += lAnimStack->GetName();
			lOutputString += "\n\n";
			FBXSDK_printf(lOutputString);

			DisplayAnimation(lAnimStack, pFbxScene->GetRootNode(),skaData,skeData);

			break;
		}

		ConverteAnimDataParentToLocalSpaceAnimation(&skaData,&skeData);

		skaData.SaveToFile(pOutSkaFile);
	}

	bool LoadAnimationData(const char* pFileName, Animation& skaData,const Skeleton& skeData)
	{
		if (pFileName == NULL)
			return false;

		FbxScene* pFbxScene = GetFbxScene(pFileName);
		if (pFbxScene == NULL)
			return false;

		int nAnimStackCount = pFbxScene->GetSrcObjectCount<FbxAnimStack>();
		ASSERT(nAnimStackCount == 1);
		for (int i = 0; i < nAnimStackCount; i++)
		{
			FbxAnimStack* lAnimStack = pFbxScene->GetSrcObject<FbxAnimStack>(i);

			FbxString lOutputString = "Animation Stack Name: ";
			lOutputString += lAnimStack->GetName();
			lOutputString += "\n\n";
			FBXSDK_printf(lOutputString);

			DisplayAnimation(lAnimStack, pFbxScene->GetRootNode(),skaData,skeData);

			break;
		}

		ConverteAnimDataParentToLocalSpaceAnimation(&skaData,&skeData);

		//m_pFBXImporter->Destroy();

		return true;
	}


}
