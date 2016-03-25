#ifndef __LookAtModifier_H__
#define __LookAtModifier_H__

#include "Animation/PoseModifier/PoseModifier.h"

namespace ma
{
	class LookAtModifier : public PoseModifier
	{
	public:
		LookAtModifier();

		~LookAtModifier();

		DECL_OBJECT(LookAtModifier)

		static void			RegisterAttribute();

		void				UpdatePose(SkeletonPose* pNodePose,Skeleton* pSkeleton,float fWeight);

		void				SetGoalObjectSpace(const Vector3& vGoalOS) {m_vGoalOS = vGoalOS;}
		const Vector3&		GetGoalObjectSpace()const {return m_vGoalOS;}

		void				SetBoneName(const char* pszName) {m_strBoneName = pszName ? pszName : "";}
		const char*			GetBoneName() const {return m_strBoneName.c_str();}

		void				SetMaxYaw(float fMaxYaw) {m_fMaxYaw = Math::Clamp(fMaxYaw,0.0f,Math::PI);}
		float				GetMaxYaw() const {return m_fMaxYaw;}
	
		void				SetFowardLS(const Vector3& vForwardLS) {m_vForwardLS = vForwardLS;}
		const Vector3& 		GetFowardLS() const {return m_vForwardLS;}

		void				SetUpLs(const Vector3& vUpLs) {m_vUpLs = vUpLs;}
		const Vector3&		GetUpLS() const {return m_vUpLs;}

		void				DbgDraw() const;


	private:
		Vector3				m_vGoalOS;	

		Quaternion			m_qPivotLS;

		UINT				m_nBoneID;
		
		string				m_strBoneName;

		Vector3				m_vForwardLS;
		
		Vector3				m_vUpLs;

		float				m_fGoalDistThreshold;

		float				m_fMaxYaw;

		bool				m_bInit;
	};

	RefPtr<LookAtModifier> CreateLookAtModifier();
}



#endif// __LookAtModifier_H__