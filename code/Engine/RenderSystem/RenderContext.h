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

		void					SetViewMatrix(const Matrix4& matView);
		const Matrix4&			GetViewMatrix() const;
		const Matrix4&			GetViewMatrixInv() const;
		void					SetProjMatrix(const Matrix4& matProj);
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
		MatViewProj				m_matViewProj[2];

		float					m_fNear[2];
		float					m_fFar[2];
		Vector3					m_vEyeWordPos[2];

		Light*					m_pCurLight;
	};

	RenderContext*	GetRenderContext();

	void			SetRenderContext(RenderContext* pRenderContext);
}

