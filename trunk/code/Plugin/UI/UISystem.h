#ifndef  _UISystem__H__
#define  _UISystem__H__

namespace ma
{
	class UI_API UISystem : public IUISystem
	{
	public:
		UISystem();

		~UISystem();

		virtual void	Init();

		virtual void	Shutdown();

		virtual void	Update();

		virtual void	Render();

		virtual void	OnFlushFrame();

		Form*			Create(const char* url);

		Form*			Create(const char* id, Theme::Style* style, Layout::Type layoutType);

	private:
		std::vector<Form*>		m_arrForms;

		StringRender*			m_pStringRender;
	};

}

#endif