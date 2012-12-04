#ifndef  _AnimationRes__H__
#define  _AnimationRes__H__

#include "Resource.h"

namespace ma
{
	class Animation;

	class FRAMEWORK_API AnimationRes : public Resource
	{
	public:
		AnimationRes(const char* pszPath = NULL);

		~AnimationRes();

		void Load();

		bool IsLoad();

		Animation* GetAimation() {return m_pAnimation;}

	private:
		Animation* m_pAnimation;
	};
}




#endif