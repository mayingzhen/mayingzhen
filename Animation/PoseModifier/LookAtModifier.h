#ifndef __LookAtModifier_H__
#define __LookAtModifier_H__

#include "Animation/PoseModifier/PoseModifier.h"

namespace ma
{
	class ANIMATION_API LookAtModifier : public PoseModifier
	{
		xmVector3 m_vGoalOS;		//goal in object space

		xmQuaternion	m_qPivotLS;

		xmUint		m_nBoneID;

		float		m_fGoalDistThreshold;

		float		m_fMaxYaw;

	public:

		LookAtModifier();

		void				UpdatePose(NodePose* pNodePose);

		void				Init(xmUint nBoneID,const xmVector3& vFowardLS,const xmVector3& vUpLS);

		void				SetBoneID(xmUint nBoneID);

		xmUint				GetBoneID()const;

		void				SetGoalObjectSpace(const xmVector3& vGoalOS);

		const xmVector3&	GetGoalObjectSpace()const;

		void				SetMaxYaw(float fMaxYaw);

		float				GetMaxYaw() const;

		xmVector3			GetFowardLS() const;

		xmVector3			GetUpLS() const;

		void				DbgDraw() const;

	private:
		void CalculatePovitLs(const xmVector3& vFowardLS,const xmVector3& vUpLS);
	};
}



#endif// __LookAtModifier_H__