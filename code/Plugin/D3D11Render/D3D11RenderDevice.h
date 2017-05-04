#pragma once

namespace ma
{
	class IndexBuffer;
	class VertexBuffer;
	class ConstantBuffer;
	class D3D11Resource;
	class D3D11ShaderProgram;
	class D3D11VertexDeclaration;

	class D3D11RenderDevice : public IRenderDevice
	{
	public:
		D3D11RenderDevice();

		~D3D11RenderDevice();

		virtual RenderDeviceType	GetRenderDeviceType() {return RenderDevice_D3D11;}

		virtual Texture*			CreateTexture();
		virtual Texture*			CreateRenderTarget(int nWidth,int nHeight,UINT nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType);
		virtual Texture*			CreateDepthStencil(int nWidth,int nHeight,PixelFormat format,bool bTypeLess);
		virtual VertexDeclaration*	CreateVertexDeclaration();
		virtual VertexBuffer*		CreateVertexBuffer();
		virtual IndexBuffer*		CreateIndexBuffer();
		virtual ShaderProgram*		CreateShaderProgram();

		virtual BlendState*			CreateBlendState();
		virtual DepthStencilState*	CreateDepthStencilState();
		virtual RasterizerState*	CreateRasterizerState();

		virtual SamplerState*		CreateSamplerState();

		virtual	void				SetFrameBuffer(FrameBuffer* pFB);
		virtual	void				SetRenderTarget(int index,Texture* pTexture,int level = 0, int array_index = 0, int face = 0);
		virtual	Texture*			GetDefaultRenderTarget(int index = 0);
		virtual void				SetDepthStencil(Texture* pTexture);
		virtual Texture*			GetDefaultDepthStencil();
		virtual void				SetViewport(const Rectangle& rect);
		virtual Rectangle			GetViewport();

		virtual void				SetBlendState(const BlendState* pBlendState);
		virtual void				SetDepthStencilState(const DepthStencilState* pDSState);
		virtual void				SetRasterizerState(const RasterizerState* pRSState);
		
		virtual void				SetValue(Uniform* uniform, int value);
		virtual void				SetValue(Uniform* uniform, float value);
		virtual void				SetValue(Uniform* uniform, const Vector2& value);
		virtual void				SetValue(Uniform* uniform, const Vector3& value);
		virtual void				SetValue(Uniform* uniform, const Vector4* values, UINT count);
		virtual void				SetValue(Uniform* uniform, const Matrix4* values, UINT count);
		virtual void				SetValue(Uniform* uniform, const ColourValue& value);
		virtual	void				SetTexture(Uniform* uniform,Texture* pTexture);
		virtual	void				SetTexture(uint32 nIndex,Texture* pTexture,bool bSRGBNotEqual);
		virtual void				SetSamplerState(Uniform* uniform,SamplerState* pTexture);

		virtual void				SetShaderProgram(ShaderProgram* pShader);
		virtual	void				SetVertexDeclaration(const VertexDeclaration* pDec);
		virtual void				SetIndexBuffer(IndexBuffer* pIB);
		virtual	void				SetVertexBuffer(int index, VertexBuffer* pVB);

		virtual void				DrawRenderable(const Renderable* pRenderable,Technique* pTech);

		virtual	void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s);

		virtual	void				Init(HWND wndhandle);
		virtual void				Shoutdown();
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
		virtual	bool				CheckTextureFormat(PixelFormat eFormat,TEXTURE_USAGE eUsage);

		ID3D11Device*				GetDXDevive() {return m_pD3DDevice;}
		ID3D11DeviceContext*		GetDXDeviveContext() {return m_pDeviceContext;}

		void						NotifyResourceCreated(D3D11Resource* pRes);

		void						NotifyResourceDestroyed(D3D11Resource* pRes);

		void						ClearAllStates();

		ConstantBuffer*				GetOrCreateConstantBuffer(ShaderType type, unsigned index, unsigned size);

		//void						CreateOrGetDepthStencilState(const DepthStencilState* pDSState);

	private:
		void						SetValue(Uniform* uniform, const float* values, UINT count);

		void						CommitChanges();
		
		bool						BuildDeviceCapabilities();

		bool						UpdateSwapChain(int width, int height);

		//ID3D11DepthStencilState*	CreateOrGetSamplerState(SamplerState* pSampler);

		void						DetachSRV(ID3D11ShaderResourceView* rtv_src);
	
	private:
		ID3D11Device*				m_pD3DDevice;

		/// Immediate device context.
		ID3D11DeviceContext*		m_pDeviceContext;

		IDXGISwapChain*				m_pSwapChain;

		HWND						m_hWnd;


		RefPtr<D3D11Texture>		m_pDefaultRenderTargetTexture;
		RefPtr<D3D11Texture>		m_pDefaultDepthStencilTexture;


		ID3D11RenderTargetView*		m_pRenderTarget[MAX_RENDERTARGETS];
		ID3D11DepthStencilView*		m_pDepthStencil;
		bool						m_bRenderTargetsDirty;
		
		//RenderState					m_renderState;

		// RasterizerState
		bool						m_bRasterizerStateDirty;
		unsigned					m_nRasterizerStateHash;
		map<unsigned, ID3D11RasterizerState*> m_rasterizerStatePool;
		
		// DepthState
		bool						m_bDepthStateDirty;
		unsigned					m_nDepthStateHash;
		map<unsigned, ID3D11DepthStencilState*> m_depthStatePool;
		unsigned m_nStencilRef;
		bool m_bStencilRefDirty;
		ID3D11DepthStencilState* m_pCurDSState;

		// BlendState
		bool						m_bBlendStateDirty;
		unsigned					m_nBlendStateHash;
		map<unsigned, ID3D11BlendState*> m_blendStatePool;
		ID3D11BlendState*			m_pCurBlendState;
		

		// ConstantBuffer
		map<unsigned, RefPtr<ConstantBuffer> > m_mapConstantBufferPool;
		vector<ConstantBuffer*> m_vecDirtyConstantBuffers;
		ID3D11Buffer* m_vecD3D11ConstantBuffers[2][MAX_SHADER_PARAMETER_GROUPS];	

		D3D11ShaderProgram* m_pShader;

		//VertexDeclaration
		D3D11VertexDeclaration*	m_pVertexDecl;
		bool m_bVertexDeclarationDirty;
		uint64 m_nVertexDeclarationHash;
		map<uint64, ID3D11InputLayout* > m_mapVertexDeclaration;
		ID3D11InputLayout* m_pCurInput;

		IndexBuffer* m_pIndexBuffer;

		//Vertex buffers
		unsigned m_nFirstDirtyVB;
		unsigned m_nLastDirtyVB;
		ID3D11Buffer* m_arrVertexBuffers[MAX_VERTEX_STREAMS];
		unsigned m_arrVertexSize[MAX_VERTEX_STREAMS];
		unsigned m_arrVertexOffset[MAX_VERTEX_STREAMS];
		//unsigned elementMasks_[MAX_VERTEX_STREAMS];

		// 
		Texture* m_arrTexture[MAX_TEXTURE_UNITS];
		unsigned m_nFirstDirtyTexture;
		unsigned m_nLastDirtyTexture;
		bool	m_bTexturesDirty;
		ID3D11ShaderResourceView* m_arrShaderResourceView[MAX_TEXTURE_UNITS];

		SamplerState* m_arrSamplerState[MAX_TEXTURE_UNITS];
		unsigned m_nFirstDirtySamplerState;
		unsigned m_nLastDirtySamplerState;
		bool	m_bSamplerStatesDirty;
		ID3D11SamplerState* m_arrD3d11Sampler[MAX_TEXTURE_UNITS];
		map<SamplerState, ID3D11SamplerState*> m_SamplerStatesPool;

	};

	ID3D11Device* GetD3D11DxDevive();
	ID3D11DeviceContext* GetD3D11DxDeviveContext();
}


