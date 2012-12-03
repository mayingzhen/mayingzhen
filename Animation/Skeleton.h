#ifndef  _SKELETON__H__
#define  _SKELETON__H__

#include "Animation/NodePose.h"

class SkeletonData;

namespace ma
{
	class ANIMATION_API Skeleton
	{
	public:
		Skeleton();

		~Skeleton();

		bool Load(const char* pszPath);

		const D3DXMATRIX& GetBoneMatrixOSInv(BoneIndex nBoneID);

	private:
		void Init(const SkeletonData& skelData);

	private:
		std::string m_name;

		std::vector<std::string> m_arrBoneName;
		std::vector<BoneIndex>	 m_arrParentInd;

		NodePose				 m_refPose;

		// D3DXMATRIX
		std::vector<D3DXMATRIX> m_arrRefPosePS;
		std::vector<D3DXMATRIX> m_arrRefPoseOS;
		std::vector<D3DXMATRIX> m_arrRefPoseOSInv;
	};
}



#endif