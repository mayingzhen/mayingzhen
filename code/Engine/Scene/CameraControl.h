#pragma once
#include "Mouse.h"
#include "Keyboard.h"

namespace ma
{

class CCameraControl
{
public:
	CCameraControl(void);
	~CCameraControl(void);

	bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);
	void keyEvent(Keyboard::KeyEvent evt, int key);
	void Process(uint32 dwElapsed);

	enum MODE
	{
		MODE_FREE,
		MODE_PIVOT,
	};
	void SetMode(MODE eMode){m_eMode = eMode;}
	MODE GetMode() const{return m_eMode;}
	void SetPivot(const Vector3& v){m_vPivot = v;}
	void SetSpeed(float nMultiple);
	void SetAltDown(bool b){m_bAltDown = b;}
private:
	void Yaw(const Radian& r);
	void Pitch(const Radian& r);
private:
	float m_fMoveSpeed;
	float m_fRotateSpeed;
	bool m_bAltDown;

	enum
	{
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3,
		YAW_LEFT = 4,
		YAW_RIGHT = 5,
		HEIGHT_UP = 6,
		HEIGHT_DOWN = 7,
		FORWARD = 8,
		BACKWARD = 9,
		PITCH_UP = 10,
		PITCH_DOWN = 11,
		KEY_AMOUNT,
	};
	bool m_rgKeyDown[KEY_AMOUNT];

	MODE m_eMode;
	Vector3 m_vPivot;

	Camera* m_pCamera;
};

}