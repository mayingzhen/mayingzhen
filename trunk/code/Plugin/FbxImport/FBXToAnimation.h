#ifndef  _FbxImportAnimation__H__
#define  _FbxImportAnimation__H__

namespace ma
{
	struct TrackData
	{
		Vector3Track posTrack;
		QuaternionTrack RotTrack;
		Vector3Track scaleTrack;
	};

	class Animation;
	class Skeleton;

	void LoadAnimationData(FbxAnimLayer* pAnimLayer, FbxNode* pNode,FbxAnimStack* pAnimStack,std::map<std::string,TrackData>& boneTrack);
	
	void LoadAnimationData(FbxAnimStack* pAnimStack, FbxNode* pNode,Animation& skaData,const Skeleton& skeData);
	
	bool LoadAnimationData(const char* pFileName, Animation& skaData,const Skeleton& skeData);
	
}

#endif
