#ifndef _Sample_CharaControl_H_
#define _Sample_CharaControl_H_

namespace ma
{
	class Scene;

	class SampleCharaControl : public Sample,OIS::MouseListener
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

	private:
		void			MoveToWS(Vector3 vMouveTo);
		
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

