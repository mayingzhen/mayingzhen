#include "RenderState.h"

namespace ma
{
	void RenderState::Serialize(Serializer& sl, const char* pszLable/* = "RenderState"*/)
	{
		sl.BeginSection(pszLable);


		sl.EndSection();
	}

}