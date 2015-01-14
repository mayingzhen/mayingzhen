#include "FBXToAnimation.h"
#include "FbxImportUtil.h"

namespace ma
{
	void LoadAnimationData(FbxAnimLayer* pAnimLayer, FbxNode* pNode,FbxAnimStack* pAnimStack,std::map<std::string,TrackData>& boneTrack)
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

				trackData.posTrack.Pushback( i, ToMaUnit(pTranslation) );
				trackData.RotTrack.Pushback( i, ToMaUnit(pRotation) );
				trackData.scaleTrack.Pushback( i, ToMaUnit(pScaling) );

				++i;
			}

			boneTrack[pszBoneName] = trackData;	
		}

		for(lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
		{
			LoadAnimationData(pAnimLayer, pNode->GetChild(lModelCount),pAnimStack,boneTrack);
		}
	}

	void LoadAnimationData(FbxAnimStack* pAnimStack, FbxNode* pNode,Animation& skaData,const Skeleton& skeData)
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

		
			LoadAnimationData(lAnimLayer, pNode, pAnimStack, boneTrack);

			break;
		}
		
		ASSERT(boneTrack.size() == skeData.GetBoneNumer());

 		for (UINT i = 0; i < skeData.GetBoneNumer(); ++i)
		{
			std::string boneName = skeData.GetBoneNameByIndex(i);
			std::map<std::string,TrackData>::iterator it = boneTrack.find( boneName.c_str() );
			ASSERT(it != boneTrack.end());
			if (it == boneTrack.end())
				continue;

			skaData.AddTrack(boneName.c_str(),it->second.scaleTrack,it->second.RotTrack,it->second.posTrack);
		}
	}

	bool LoadAnimationData(const char* pFileName,const Skeleton& skeData,const char* pOutSkaFile)
	{
		if (pFileName == NULL)
			return false;

		std::string strDir = GetArchiveMananger()->GetArchiveByIndex(0)->GetName();	
		std::string strAnimFile = strDir + "/" + string(pFileName);

		FbxScene* pFbxScene = GetFbxScene(strAnimFile.c_str());
		if (pFbxScene == NULL)
			return false;

		RefPtr<Animation> pSkaData = CreateObject<Animation>();

		int nAnimStackCount = pFbxScene->GetSrcObjectCount<FbxAnimStack>();
		ASSERT(nAnimStackCount == 1);
		for (int i = 0; i < nAnimStackCount; i++)
		{
			FbxAnimStack* lAnimStack = pFbxScene->GetSrcObject<FbxAnimStack>(i);

			FbxString lOutputString = "Animation Stack Name: ";
			lOutputString += lAnimStack->GetName();
			lOutputString += "\n\n";
			FBXSDK_printf(lOutputString);

			LoadAnimationData(lAnimStack, pFbxScene->GetRootNode(),*pSkaData,skeData);

			break;
		}

		pSkaData->ConverteAnimDataParentToLocalSpace(&skeData);

		pSkaData->SaveToFile(pOutSkaFile);

		return true;
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

			LoadAnimationData(lAnimStack, pFbxScene->GetRootNode(),skaData,skeData);

			break;
		}

		skaData.ConverteAnimDataParentToLocalSpace(&skeData);

		//m_pFBXImporter->Destroy();

		return true;
	}


}
