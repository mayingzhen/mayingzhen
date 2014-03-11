#ifndef  _AnimationSetData__H__
#define  _AnimationSetData__H__

#include "ActionData.h"
#include "BoneSet.h"
#include "PoseModifier/PoseModifier.h"

namespace ma
{
// 	class BoneSet;
// 	class PoseModifier;
// 	class ActionData;

	class ANIMATION_API AnimationSetData : public Resource
	{
		DECL_OBJECT(AnimationSetData)

	public:
		~AnimationSetData();

		virtual void		SaveToFile(const char* pszPath);

		virtual bool		CreateFromMemeory();

		virtual void		Serialize(Serializer& sl, const char* pszLable = "AnimationSet");

		UINT				GetActionDataNumber() {return m_arrActionData.size();}

		ActionData*			GetActionDataByIndex(UINT index) {return m_arrActionData[index];}

		void				AddAnimClip(const char* skaPath,const char* pszActionName);

	private:
		std::vector<BoneSet*>			m_arrBoneSet;

		std::vector<PoseModifier*>		m_arrPoseModifier;

		std::vector<ActionData*>		m_arrActionData;
	};
}


#endif
