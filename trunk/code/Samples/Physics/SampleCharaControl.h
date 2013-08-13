#ifndef _Sample_CharaControl_H_
#define _Sample_CharaControl_H_

namespace ma
{
	class Scene;

	class SampleCharaControl : public Sample,OIS::MultiTouchListener,OIS::MouseListener
	{
	public:
		SampleCharaControl();

		virtual void	Load();

		virtual void	UnLoad();

		virtual void	Update();

		virtual void	Render();

		virtual bool	mouseMoved( const OIS::MouseEvent &arg );
		virtual bool	mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool	mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool	touchMoved( const OIS::MultiTouchEvent &arg );
		virtual bool	touchPressed( const OIS::MultiTouchEvent &arg );
		virtual bool	touchReleased( const OIS::MultiTouchEvent &arg );
		virtual bool	touchCancelled( const OIS::MultiTouchEvent &arg );

	private:
		void			OnTouch(Vector2 vTouchPos);
		
		void			DoMouveTo();

	private:
		GameObject*		m_pCharaObj;

		GameObject*		m_pTerrain;
	
		Vector3			m_vMoveTo;

		float			m_fMoveSpeed;

		bool			m_bMoveing;
	};
}


#endif

