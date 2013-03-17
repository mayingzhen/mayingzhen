#ifndef  _Light__H__
#define  _Light__H__

#include "Engine/Scene/SceneNode.h"

namespace ma
{
	enum LightType
	{
		TYPE_ORTHOGRAPHIC, // eg. directional light
		TYPE_PERSPECTIVE,   // eg. spot light
		TYPE_POINT,	
	};


	class ENGINE_API Light : public SceneNode
	{
	public:
		Light(Scene* pSene = NULL, const char* pNodeName = NULL);

		virtual	void	Update();

		LightType		GetLightType() {return m_Type;}

		void			SetLigtType(LightType eType) {m_Type = eType;}

		bool			IsCreateShadow() {return m_bCreateShadow;}

		void			SetCeateShadow(bool bCreateShadow) {m_bCreateShadow = bCreateShadow;}

		D3DXMATRIX		GetViewMatrix() {return m_mView;}

		D3DXMATRIX		GetProjmatrix()	{return m_mProj;}

		D3DXVECTOR3		GetDiffuse() {return m_vLightDiffuse;}

		float			GetRadius() {return m_fRange;}

		D3DXVECTOR3		GetDir() {return m_vSource - m_vTarget;}

		float			GetNearClip() {return m_fNear;}

		float			GetFarClip() {return m_fFar;}

	protected:
		void			CalculateMatrices();

	protected:
		//D3DXCOLOR	m_Diffuse;
		D3DXCOLOR   m_fSpecular;
		float       m_fRange;
		float       m_fFalloff;
		float       m_fAtten0;
		float       m_fAtten1;
		float       m_fAtten2;
		float       m_fInner;
		float       m_fMOuter;

		// type of light
		LightType m_Type;

		D3DXMATRIX m_mView;
		D3DXMATRIX m_mProj;


		bool		m_bCreateShadow;

		D3DXVECTOR3 m_vSource;
		D3DXVECTOR3 m_vTarget;
		float m_fNear;
		float m_fFar;
		float m_fFOV;
		D3DXVECTOR3 m_vUpVector;
		float m_fAspectRatio;
		D3DXVECTOR3 m_vLightDiffuse;
		D3DXVECTOR3 m_vLightAmbient;
	};
}

#endif

