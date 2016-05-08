#ifndef  _FbxImportAnimation__H__
#define  _FbxImportAnimation__H__

namespace ma
{
	struct TrackData : public Referenced
	{
		RefPtr<Vector3Track> posTrack;
		RefPtr<QuaternionTrack> RotTrack;
		RefPtr<Vector3Track> scaleTrack;
	};

	class Animation;
	class Skeleton;

	void LoadAnimationData(FbxAnimLayer* pAnimLayer, FbxNode* pNode,FbxAnimStack* pAnimStack,std::map<std::string,RefPtr<TrackData> >& boneTrack);
	
	void LoadAnimationData(FbxAnimStack* pAnimStack, FbxNode* pNode,Animation& skaData,const Skeleton& skeData);
	
	bool LoadAnimationData(const char* pFileName, Animation& skaData,const Skeleton& skeData);
	
}

#endif
