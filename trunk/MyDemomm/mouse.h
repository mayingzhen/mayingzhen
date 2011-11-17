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

	CSprite *m_mouseCommon; //��ͨ��̬���
	CSprite *m_mouseDiaog;  //�Ի���̬���
	CSprite *m_mouseFight;	//ѡ��������	

	int m_x;  //����ڴ���������
	int m_y;

	MOUSE_TYPE m_mouseType;
};

#endif