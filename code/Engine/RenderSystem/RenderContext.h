#pragma once


namespace ma
{
	class Light;
	class Camera;

	class SceneContext : public Referenced
	{
	public:
		SceneContext();
		
		void					SetCamera(Camera* pCamera); 

		void					SetMainLight(Light* pMianLight, ColourValue cAmbientColor);

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

	protected:
		MatViewProj				m_matViewProj;

		float					m_fNear = 0.0f;
		float					m_fFar = 0.0f;
		Vector3					m_vEyeWordPos = Vector3::ZERO;

		Vector3					m_vMainLightDir;
		ColourValue				m_cMianLightColor;
		ColourValue				m_cAmbientColor;

	};

	SceneContext*	GetSceneContext();

	void			SetSceneContext(SceneContext* pRenderContext);
}

