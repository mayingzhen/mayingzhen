#include "UI/stdafx.h"
#include "Module.h"


#include "UI/Control.hxx"
#include "UI/Layout.hxx"
#include "UI/FlowLayout.hxx"
#include "UI/AbsoluteLayout.hxx"
#include "UI/VerticalLayout.hxx"
#include "UI/CheckBox.hxx"
#include "UI/Container.hxx"
#include "UI/Font.hxx"
#include "UI/Form.hxx"
#include "UI/Label.hxx"
#include "UI/Button.hxx"
#include "UI/RadioButton.hxx"
#include "UI/ImageControl.hxx"
#include "UI/Theme.hxx"
#include "UI/ThemeStyle.hxx"
#include "UI/Slider.hxx"
#include "UI/TextBox.hxx"
#include "UI/Bundle.hxx"
#include "UI/UISystem.hxx"

using namespace ma;

void UIModuleInit()
{
	UISystem* pUISystem = new UISystem();
	SetUISystem(pUISystem);
}

void UIModuleShutdown()
{
	IUISystem* pUISystem = GetUISystem();
	SAFE_DELETE(pUISystem);
}


#if PLATFORM_WIN == 1
extern "C" UI_API bool dllStartPlugin()
{
	UIModuleInit();

	return true;
}


extern "C" UI_API bool dllStopPlugin()
{
	UIModuleShutdown();

	return true;
}
#endif
