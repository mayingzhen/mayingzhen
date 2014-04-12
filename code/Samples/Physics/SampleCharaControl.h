#ifndef _Sample_CharaControl_H_
#define _Sample_CharaControl_H_

namespace ma
{
	class Scene;

	class SampleCharaControl : public Sample
	{
	public:
		SampleCharaControl();

		virtual void	Load();

		virtual void	UnLoad();

		virtual void	Update();

		virtual void	Render();

		virtual bool	mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

		virtual bool	touchReleased( const OIS::MultiTouchEvent &arg );

	private:
		void			OnTouch(Vector2 vTouchPos);
		
		void			DoMouveTo();

	private:
		GameObjectPtr			m_pTerrain;

		AnimationComponentPtr	m_pCharaAnim;
	
		Vector3					m_vMoveTo;

		float					m_fMoveSpeed;

		bool					m_bMoveing;
	};
}


#endif

