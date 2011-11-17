#ifndef C_MOUSE_HH_
#define C_MOUSE_HH_ 

#include "public.h"

class CSprite ;
class CSkinMesh;

enum MOUSE_TYPE {COMMON,DIAOG,FIGHT};

class CMouse : public CSingleton<CMouse>
{
public:
	CMouse();
	~CMouse();

	void Render();
	void UpdateMousePos();
	void LoadMousePicture();

	bool MouseTest(CSkinMesh *mesh);

	void SetMouseType(MOUSE_TYPE mouseType);
private:
	void CleanUp();

	CSprite *m_mouseCommon; //普通形态鼠标
	CSprite *m_mouseDiaog;  //对话形态鼠标
	CSprite *m_mouseFight;	//选择怪物鼠标	

	int m_x;  //鼠标在窗口上坐标
	int m_y;

	MOUSE_TYPE m_mouseType;
};

#endif