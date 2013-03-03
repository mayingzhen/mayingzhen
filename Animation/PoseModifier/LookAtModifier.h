#ifndef __LookAtModifier_H__
#define __LookAtModifier_H__

#include "Animation/PoseModifier/PoseModifier.h"

namespace ma
{
	class ANIMATION_API LookAtModifier : public PoseModifier
	{
		D3DXVECTOR3 m_vGoalOS;		//goal in object space

		D3DXQUATERNION	m_qPivotLS;

		UINT		m_nBoneID;

		float		m_fGoalDistThreshold;

		float		m_fMaxYaw;

	public:

		LookAtModifier();

		void				UpdatePose(NodePose* pNodePose);

		void				Init(UINT nBoneID,const D3DXVECTOR3& vFowardLS,const D3DXVECTOR3& vUpLS);

		void				SetBoneID(UINT nBoneID);

		UINT				GetBoneID()const;

		void				SetGoalObjectSpace(const D3DXVECTOR3& vGoalOS);

		const D3DXVECTOR3&	GetGoalObjectSpace()const;

		void				SetMaxYaw(float fMaxYaw);

		float				GetMaxYaw() const;

		D3DXVECTOR3			GetFowardLS() const;

		D3DXVECTOR3			GetUpLS() const;

		void				DbgDraw() const;

	private:
		void CalculatePovitLs(const D3DXVECTOR3& vFowardLS,const D3DXVECTOR3& vUpLS);
	};
}



#endif// __LookAtModifier_H__