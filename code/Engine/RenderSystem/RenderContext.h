#pragma once


namespace ma
{
	class Light;
	class Camera;

	class RenderContext : public Referenced
	{
	public:
		RenderContext();

		void					AddLight(Light* pLight);
		
		void					SetCamera(Camera* pCamera); 

		const Matrix4&			GetViewMatrix() const;
		const Matrix4&			GetViewMatrixInv() const;
		const Matrix4&			GetProjMatrix() const;
		const Matrix4&			GetViewProjMatrix() const;
		const Vector3&			GetEyeWorldPos() const;

		float					GetNearClip();
		float					GetFarClip();

		Vector3					GetAmbientColor() const;
		Vector3					GetDirLightColor() const;
		Vector3					GetDirLightDir() const;

		void					Clear();

	protected:
		MatViewProj				m_matViewProj;

		float					m_fNear = 0.0f;
		float					m_fFar = 0.0f;
		Vector3					m_vEyeWordPos = Vector3::ZERO;

		Light*					m_pCurLight;
	};

	RenderContext*	GetRenderContext();

	void			SetRenderContext(RenderContext* pRenderContext);
}

