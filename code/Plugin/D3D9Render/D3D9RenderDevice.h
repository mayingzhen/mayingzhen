#ifndef  _D3D9RENDERDevice__H__
#define  _D3D9RENDERDevice__H__

namespace ma
{
	class IndexBuffer;
	class VertexBuffer;
	class D3D9Resource;

	#define DEFINE_ALIGNED_DATA( type, name, alignment ) _declspec(align(alignment)) type name;
	#define MAX_CONSTANTS_PS 64
	#define MAX_CONSTANTS_VS 256
	#define MERGE_SHADER_PARAMETERS 1

	class D3D9RenderDevice : public IRenderDevice
	{
	public:
		D3D9RenderDevice();

		~D3D9RenderDevice();

		virtual RenderDeviceType	GetRenderDeviceType() {return RenderDevice_D3D9;}

		virtual Texture*			CreateTexture(const char* pszPath = NULL);
		virtual Texture*			CreateTexture(int nWidth,int nHeight,PixelFormat format = PF_A8R8G8B8,USAGE eUsage = USAGE_STATIC);
		virtual VertexDeclaration*	CreateVertexDeclaration();
		virtual VertexBuffer*		CreateVertexBuffer();
		virtual IndexBuffer*		CreateIndexBuffer();
		virtual ShaderProgram*		CreateShaderProgram();

		virtual	void				SetRenderTarget(Texture* pTexture,int index = 0);
		virtual	Texture*			GetRenderTarget(int index = 0);
		virtual void				SetDepthStencil(Texture* pTexture);
		virtual Texture*			GetDepthStencil();
		virtual void				SetViewport(const Rectangle& rect);
		virtual Rectangle			GetViewport();

		virtual	void				SetDepthBias(float constantBias, float slopeScaleBias = 0.0f);
		virtual	void				SetCullingMode(CULL_MODE mode);
		virtual void				SetBlendMode(BLEND_MODE mode);
		virtual	void				SetDepthCheckMode(DEPTH_CHECK_MODE mode);
		virtual	void				SetDepthWrite(bool b);
		virtual	void				SetColorWrite(bool b);

		virtual	void				SetTexture(Uniform* uniform,Texture* pTexture);
		virtual	void				SetTextureWrap(Uniform* uniform,Wrap eWrap);
		virtual void				SetTextureFilter(Uniform* uniform,FilterOptions eFilter);
		
		virtual void				SetValue(Uniform* uniform, float value);
		virtual void				SetValue(Uniform* uniform, const Vector2& value);
		virtual void				SetValue(Uniform* uniform, const Vector3& value);
		virtual void				SetValue(Uniform* uniform, const Vector4* values, UINT count);
		virtual void				SetValue(Uniform* uniform, const Matrix4* values, UINT count);
		virtual void				SetValue(Uniform* uniform, const ColourValue& value);
		
		virtual	void				SetVertexDeclaration(VertexDeclaration* pDec);
		virtual void				SetIndexBuffer(IndexBuffer* pIB);
		virtual	void				SetVertexBuffer(int index, VertexBuffer* pVB);

		virtual void				DrawRenderable(const Renderable* pRenderable,Technique* pTech);
		virtual void				DrawDyRenderable(const Renderable* pRenderable,Technique* pTech);

		virtual	void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s);

		virtual	void				Init(HWND wndhandle);
		virtual void				ShutDown();
		virtual bool				TestDeviceLost();
		virtual	bool				Rest();
		virtual void				BeginRender();
		virtual void				EndRender();
		
		virtual	Matrix4				MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf);
		virtual	Matrix4				MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf);
		virtual Matrix4				MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf);

		virtual	void				BeginProfile(const char* pszLale);
		virtual	void				EndProfile();

		// Help fun
		virtual	bool				CheckTextureFormat(PixelFormat eFormat,USAGE eUsage);
		virtual void				ConvertUV(float& fTop,float& fLeft,float& fRight,float& fBottom);
		virtual float				GetHalfPixelOffset(float fHalfPiexl);

		LPDIRECT3DDEVICE9			GetDXDevive() {return m_pD3DDevice;}

		void						NotifyResourceCreated(D3D9Resource* pRes);

		void						NotifyResourceDestroyed(D3D9Resource* pRes);

		void						ClearAllStates();

	private:
		void						SetValue(Uniform* uniform, const float* values, UINT count);
		void						mfSetPSConst(int nReg, const float *vData, const int nParams);
		void						mfSetVSConst(int nReg, const float *vData, const int nParams);
		void						CommitChanges();
		
		bool						BuildDeviceCapabilities();
	
	private:
		LPDIRECT3D9					m_pD3D9;			 // The main D3D object
		UINT						m_nAdapterToUse;
		D3DPRESENT_PARAMETERS		m_d3dpp;         // Parameters for CreateDevice/Reset
		LPDIRECT3DDEVICE9			m_pD3DDevice;
		D3DCAPS9					m_d3dcaps;
		HWND						m_hWnd;


		typedef std::vector<D3D9Resource*> VEC_D3D9RESOURCE;
		VEC_D3D9RESOURCE			m_vecD3D9Resource;


		// Const buffer
		DEFINE_ALIGNED_DATA(Vector4, m_CurVSParams[MAX_CONSTANTS_VS], 16);
		DEFINE_ALIGNED_DATA(Vector4, m_CurPSParams[MAX_CONSTANTS_PS], 16);
		int							m_VSParamsToCommit[MAX_CONSTANTS_VS];
		int							m_NumVSParamsToCommit;
		int							m_PSParamsToCommit[MAX_CONSTANTS_PS];
		int							m_NumPSParamsToCommit;

		bool						m_bZEnable;
	};

	LPDIRECT3DDEVICE9 GetD3D9DxDevive();
}

#endif


