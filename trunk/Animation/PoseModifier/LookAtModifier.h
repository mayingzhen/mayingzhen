#ifndef __LookAtModifier_H__
#define __LookAtModifier_H__

#include "Animation/PoseModifier/PoseModifier.h"

namespace ma
{
	class ANIMATION_API LookAtModifier : public PoseModifier
	{
		DECL_OBJECT(LookAtModifier)

		Vector3 m_vGoalOS;		//goal in object space

		Quaternion	m_qPivotLS;

		UINT		m_nBoneID;

		float		m_fGoalDistThreshold;

		float		m_fMaxYaw;

	public:

		LookAtModifier();

		void				UpdatePose(SkeletonPose* pNodePose);

		void				Init(UINT nBoneID,const Vector3& vFowardLS,const Vector3& vUpLS);

		void				SetBoneID(UINT nBoneID);

		UINT				GetBoneID()const;

		void				SetGoalObjectSpace(const Vector3& vGoalOS);

		const Vector3&	GetGoalObjectSpace()const;

		void				SetMaxYaw(float fMaxYaw);

		float				GetMaxYaw() const;

		Vector3			GetFowardLS() const;

		Vector3			GetUpLS() const;

		void				DbgDraw() const;

	private:
		void CalculatePovitLs(const Vector3& vFowardLS,const Vector3& vUpLS);
	};
}



#endif// __LookAtModifier_H__