#ifndef __CameraController_H__
#define __CameraController_H__


namespace ma
{

	class CameraController
	{

	public:
		CameraController(Camera* pCamObj);

		void		UpdateInput();		
		
		void		SetMoveStep(float fMoveStep) {m_fMoveStep = fMoveStep;}

		float		GetMoveStep() const {return m_fMoveStep;}

	private:

		void		RotateMoveCamera();

		void		MoveCamera();

		void		RotateCamera();

		void		ZoomCamera(float fDeltaZoom);

		void		ComputeRotationCenter(Matrix4x4* pOut) const;

		void		UpdateTarget();

		void		UpdateTargetDis();


	private:
		float			m_fMoveStep;

		Vector3			m_vTarget;	

		float			m_fTargetDis;

		Camera*			m_pCamera;
	};
}

#endif 

