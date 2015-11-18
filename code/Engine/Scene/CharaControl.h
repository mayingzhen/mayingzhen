#pragma once

#include "Keyboard.h"
#include "Mouse.h"

namespace ma
{
	class CharaControl : public Component
	{
	public:
		CharaControl();

		~CharaControl();

		DECL_OBJECT(CharaControl);

		static void		RegisterAttribute();

		virtual	void	Process();

		virtual	void	OnAddToSceneNode(SceneNode* pGameObj);
	
		float GetMoveSpeed() const {return m_fMoveSpeed;}
		void SetMoveSpeed(float fMoveSpeed) {m_fMoveSpeed = fMoveSpeed;}

		float GetMoveFrameLength() const;
		void SetMoveFrameLength(float fMoveFrameLength);

		float GetCharScale() const;
		void SetCharScale(float fScale);

		void SetBaseScale(float fScaele) {m_fCharBaseScale = fScaele;}

		const char* GetRunAnim() const;
		void SetRunAnim(const char* pszAnimPath);

		const char* GetIdleAnim() const;
		void SetIdleAnim(const char* pszAnimPath);

	private:
		void keyEvent(Keyboard::KeyEvent evt, int key);
		void mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

	private:
		float m_fMoveSpeed;
		
		float m_fMoveFrameLength;

		float m_fCharScale;
		float m_fCharBaseScale;

		string m_strRunAnim;

		string m_strIdleAnim;

		Vector3 m_vMoveTo;
		bool m_bMoveing;
	};

	ref_ptr<CharaControl> CreateCharaControl();
}