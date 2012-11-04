#include "Module.h"
#include "DxRender.h"

void ModuleStart()
{
	ma::DxRender* pDxRender = new ma::DxRender();
	ma::SetRender(pDxRender);

}

void ModuleStop()
{
	ma::DxRender* pDxRender = ma::GetRender();
	if (pDxRender)
	{
		delete pDxRender;
		pDxRender = NULL;
	}
}