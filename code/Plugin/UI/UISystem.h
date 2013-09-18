#ifndef  _UISystem__H__
#define  _UISystem__H__

namespace ma
{
	class UI_API UISystem : public IUISystem
	{
	public:
		Form*			Create(const char* url);

		Form*			Create(const char* id, Theme::Style* style, Layout::Type layoutType);

		void			Update();

		void			Render();

	private:
		std::vector<Form*>	m_arrForms;
	};

}

#endif