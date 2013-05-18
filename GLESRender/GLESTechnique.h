#ifndef  _GLESTechnique__H__
#define  _GLESTechnique__H__


namespace ma
{

	class Effect;

	class GLESRENDER_API GLESTechnique : public Technique
	{
	public:
		GLESTechnique();

		virtual void Load(const char* pFileName, const char* pTechName);

		virtual void Begin(UINT uShaderFlg = 0);

		virtual void CommitChanges();

		virtual void End();

		virtual void SetMatrixArray(const char* pParameter,Matrix4x4* ppMatrix, UINT Count);

		virtual void SetMatrix(const char* pParameter,Matrix4x4* pMatrix);

		virtual void SetVector(const char* pParameter, Vector4* pVector);

		virtual void SetTexture(const char* pParameter, Texture* pTexture);

		Effect* GetCurEffect() {return m_pCurEffect;}

	private:
		std::string m_strTechName;

		Effect*	m_pEffect;
		Effect* m_pEffectSkin;

		Effect* m_pCurEffect;
	};

}


#endif