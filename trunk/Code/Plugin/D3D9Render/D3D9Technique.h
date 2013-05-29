#ifndef  _D3D9Technique__H__
#define  _D3D9Technique__H__


namespace ma
{

	class D3D9RENDER_API D3D9Technique : public Technique
	{
	public:
		D3D9Technique(IDirect3DDevice9 * pDevice);

		virtual void Load(const char* pFileName, const char* pTechName);

		virtual void Begin(UINT uShaderFlg = 0);

		virtual void CommitChanges();

		virtual void End();

		virtual void SetMatrixArray(const char* pParameter,Matrix4x4* ppMatrix, UINT Count);

		virtual void SetMatrix(const char* pParameter,Matrix4x4* pMatrix);

		virtual void SetVector(const char* pParameter, Vector4* pVector);

		virtual void SetTexture(const char* pParameter, Texture* pTexture);

	private:
		ID3DXEffect*	m_pDXEffect;

		std::string		m_strTechName;

		IDirect3DDevice9*   m_pD3D9Device;

		

	};

}


#endif