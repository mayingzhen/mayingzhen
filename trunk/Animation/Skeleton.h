#ifndef  _SKELETON__H__
#define  _SKELETON__H__

#include "Animation/NodePose.h"

namespace ma
{
	class ANIMATION_API Skeleton
	{
// 		void CSkeleton::Init(const std::vector<std::string>& arrBoneName,
// 			const std::vector<BoneIndex>& arrParentInd,
// 			const std::vector<CNodeTransform>& arrNodeOS);

		const D3DXMATRIX& GetBoneMatrixOSInv(BoneIndex nBoneID);

		void Serialize(SerializeListener& sl,const char* pszLabel);

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