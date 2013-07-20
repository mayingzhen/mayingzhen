#ifndef __CameraController_H__
#define __CameraController_H__


namespace ma
{

	class CameraController
	{

	public:
		CameraController(Camera* pCamera);

		void		UpdateInput();		
					
		void		SetZoomSpeed(float fZoomSpeed) {m_fZoomSpeed = fZoomSpeed;}

		float		GetZoomSpeed() const {return m_fZoomSpeed;}

		void		SetMoveSpeed(float fMoveSpeed) {m_fMoveSpeed = fMoveSpeed;}

		float		GetMoveSpeed() const {return m_fMoveSpeed;}

	private:

		void		MoveCamera(const Vector3& vDeltaLS);

		void		RotateCamera(const EulerAngleXYZ& vDeltaAngle);

		void		ZoomCamera(float fDeltaZoom);

		void		ComputeRotationCenter(Matrix4x4* pOut) const;


	private:
		float		m_fZoomSpeed;

		float		m_fMoveSpeed;

		Camera*		m_pCamera;
	};
}

#endif 

