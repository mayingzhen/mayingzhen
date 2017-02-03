# ifndef _ParameterManager_H_
# define _ParameterManager_H_


namespace ma
{
	class Parameter;
	class MethodBinding;

	class ParameterManager
	{
	public:
		ParameterManager();

		~ParameterManager();

		void SetParameterAutoBinding(Uniform* pParam/*,AutoBinding autoBinding*/);

		void UseDefaultBing(Uniform* pParam);

	private:
		// Internal auto binding handler methods.
		const Matrix4&		autoBindingGetWorldMatrix() const;
		const Matrix4&		autoBindingGetViewMatrix() const;
		const Matrix4&		autoBindingGetProjectionMatrix() const;
		Matrix4				autoBindingGetWorldViewMatrix() const;
		Matrix4				autoBindingGetViewProjectionMatrix() const;
		Matrix4				autoBindingGetWorldViewProjectionMatrix() const;
		const Matrix4&		autoBindingGetInverseTransposeWorldMatrix() const;
		const Matrix4&		autoBindingGetInverseTransposeWorldViewMatrix() const;
		Matrix4				autoBindingGetInverseProjectionMatrix() const;
		Vector3				autoBindingGetCameraWorldPosition() const;
		Vector3				autoBindingGetCameraViewPosition() const;
		Vector4				autoBindingGetCameraNearFar() const;
		const ColourValue&	autoBindingGetLightColor() const;
		Vector3				autoBindingGetLightDirection() const;
		Vector4				autoBingingDepthNearFarInvfar() const;
		SamplerState*		autoBingingSceneDiffuse() const;
		SamplerState*		autoBingingSceneDetph() const;
		SamplerState*		autoBindingSceneNormal() const;
		SamplerState*		autoBindingTextureLightShadow() const;
		SamplerState*		autoBindingDeviceDepthMap() const;
		SamplerState*		autoBindingShadowMap() const;
		Vector4				autoBindingSpitPos() const;
		UINT				autoBindingSplitCount() const;
		const Matrix4&		autoBindingShadowMatrix() const;
		Vector4				autoBindingShadowMapTexSize() const;
		Vector4				autoBindingShadowDepthFade() const;
		float				autoBindingShadowExt() const;

	private:	
		std::map<std::string, MethodBinding*> m_autoDefaultBings;
	};

	extern ParameterManager* g_pParameterManager;
	ParameterManager*	GetParameterManager();		

}

#endif
