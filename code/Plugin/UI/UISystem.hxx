#include "UISystem.h"

namespace ma
{
	void UISystem::Update()
	{
		UINT size = m_arrForms.size();
		for (UINT i = 0; i < size; ++i)
		{
			Form* form = m_arrForms[i];
			ASSERT(form);

			if (form->isEnabled() && form->isVisible())
			{
				form->update(GetTimer()->GetFrameDeltaTime());
			}
		}
	}

	void UISystem::Render()
	{
		UINT size = m_arrForms.size();
		for (UINT i = 0; i < size; ++i)
		{
			Form* form = m_arrForms[i];
			ASSERT(form);

			if (form->isVisible())
			{
				form->draw();
			}
		}
	}

	Form* UISystem::Create(const char* url)
	{
		Form* form = Form::create(url);

		m_arrForms.push_back(form);

		return form;
	}

	Form* UISystem::Create(const char* id, Theme::Style* style, Layout::Type layoutType)
	{
		Form* form = Form::create(id,style,layoutType);

		m_arrForms.push_back(form);

		return form;
	}




}
