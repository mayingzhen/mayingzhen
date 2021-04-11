#pragma once


namespace ma
{
	class Light;
	class Camera;

	class SceneContext 
	{

	public:

		void					SetCamera(Camera* pCamera); 

		void					SetLightViewProj(const Matrix4& matLightViewProj);

		void					SetMainLight(Light* pMianLight, ColourValue cAmbientColor);

	public:

		MatViewProj				m_matViewProj;

		Matrix4					m_matLightViewProj;

		float					m_fNear = 0.0f;
		float					m_fFar = 0.0f;
		Vector3					m_vEyeWordPos = Vector3::ZERO;

		Vector3					m_vMainLightDir;
		ColourValue				m_cMianLightColor;
		ColourValue				m_cAmbientColor;

		Frustum					m_frustum;

	};

	SceneContext*	GetSceneContext();

	void			SetSceneContext(SceneContext* pRenderContext);
}

