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
		virtual Texture*			CreateTexture(int nWidth,int nHeight,PixelFormat format = PF_A8R8G8B8,TEXTURE_USAGE eUsage = USAGE_STATIC);
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
		virtual	void				SetDepthCheckMode(CompareFunction mode);
		virtual	void				SetDepthWrite(bool b);
		virtual	void				SetColorWrite(bool b);
		virtual	void				SetSRGBWrite(bool b);
		virtual void				SetStencilEnable(bool b);
		virtual void				SetStencilBufferParams(CompareFunction func = CMPF_ALWAYS_PASS, 
			uint32 refValue = 0, uint32 mask = 0xFFFFFFFF, uint32 writeMask = 0xFFFFffff,
			StencilOperation stencilFailOp = SOP_KEEP, 
			StencilOperation depthFailOp = SOP_KEEP,
			StencilOperation passOp = SOP_KEEP, 
			bool twoSidedOperation = false);
		
		virtual void				SetValue(Uniform* uniform, float value);
		virtual void				SetValue(Uniform* uniform, const Vector2& value);
		virtual void				SetValue(Uniform* uniform, const Vector3& value);
		virtual void				SetValue(Uniform* uniform, const Vector4* values, UINT count);
		virtual void				SetValue(Uniform* uniform, const Matrix4* values, UINT count);
		virtual void				SetValue(Uniform* uniform, const ColourValue& value);
		virtual	void				SetTexture(Uniform* uniform,Texture* pTexture);
		virtual void				SetSamplerState(Uniform* uniform,SamplerState* pTexture);

		virtual void				SetShaderProgram(ShaderProgram* pShader);
		virtual	void				SetVertexDeclaration(VertexDeclaration* pDec);
		virtual void				SetIndexBuffer(IndexBuffer* pIB);
		virtual	void				SetVertexBuffer(int index, VertexBuffer* pVB);

		virtual void				DrawRenderable(const Renderable* pRenderable,Technique* pTech);
		virtual void				DrawDyRenderable(const Renderable* pRenderable,Technique* pTech);

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
		virtual void				ConvertUV(float& fTop,float& fLeft,float& fRight,float& fBottom);
		virtual float				GetHalfPixelOffset(float fHalfPiexl);

		ID3D11Device*				GetDXDevive() {return m_pD3DDevice;}
		ID3D11DeviceContext*		GetDXDeviveContext() {return m_pDeviceContext;}

		void						NotifyResourceCreated(D3D11Resource* pRes);

		void						NotifyResourceDestroyed(D3D11Resource* pRes);

		void						ClearAllStates();

		ConstantBuffer*				GetOrCreateConstantBuffer(ShaderType type, unsigned index, unsigned size);

	private:
		void						SetValue(Uniform* uniform, const float* values, UINT count);

		void						CommitChanges();
		
		bool						BuildDeviceCapabilities();

		bool						UpdateSwapChain(int width, int height);

		ID3D11SamplerState*			CreateOrGetSamplerState(SamplerState* pSampler);
	
	private:
		ID3D11Device*				m_pD3DDevice;

		/// Immediate device context.
		ID3D11DeviceContext*		m_pDeviceContext;

		IDXGISwapChain*				m_pSwapChain;

		HWND						m_hWnd;

		ID3D11RenderTargetView* defaultRenderTargetView_;

		ID3D11Texture2D* defaultDepthTexture_;
		ID3D11DepthStencilView* defaultDepthStencilView_;

		//enum {MAX_RENDERTARGETS = 4};
		ID3D11RenderTargetView*		m_pRenderTarget[MAX_RENDERTARGETS];
		ID3D11DepthStencilView*		m_pDepthStencil;
		bool						m_bRenderTargetsDirty;

		typedef std::vector<D3D11Resource*> VEC_D3D11RESOURCE;
		VEC_D3D11RESOURCE			m_vecD3D11Resource;
		
		RenderState					m_renderState;

		// RasterizerState
		bool						m_bRasterizerStateDirty;
		unsigned					rasterizerStateHash_;
		map<unsigned, ID3D11RasterizerState*> rasterizerStates_;
		
		// DepthState
		bool						m_bDepthStateDirty;
		unsigned					depthStateHash_;
		map<unsigned, ID3D11DepthStencilState*> depthStates_;
		unsigned stencilRef_;
		bool stencilRefDirty_;

		// BlendState
		bool						m_bBlendStateDirty;
		unsigned					blendStateHash_;
		map<unsigned, ID3D11BlendState*> blendStates_;

		// ConstantBuffer
		map<unsigned, RefPtr<ConstantBuffer> > constantBufferAll;
		vector<ConstantBuffer*> dirtyConstantBuffers_;

		D3D11ShaderProgram* m_pShader;

		//VertexDeclaration
		D3D11VertexDeclaration*	m_pVertexDecl;
		bool vertexDeclarationDirty_;
		unsigned long long vertexDeclarationHash_;
		map<unsigned long long, ID3D11InputLayout* > vertexDeclarations_;

		IndexBuffer* indexBuffer_;

		//Vertex buffers
		//enum {MAX_VERTEX_STREAMS = 4};
		unsigned firstDirtyVB_;
		unsigned lastDirtyVB_;
		ID3D11Buffer* vertexBuffers_[MAX_VERTEX_STREAMS];
		unsigned vertexSizes_[MAX_VERTEX_STREAMS];
		unsigned vertexOffsets_[MAX_VERTEX_STREAMS];
		unsigned elementMasks_[MAX_VERTEX_STREAMS];

		// 
		Texture* textures_[MAX_TEXTURE_UNITS];
		unsigned firstDirtyTexture_;
		unsigned lastDirtyTexture_;
		bool	texturesDirty_;
		ID3D11ShaderResourceView* shaderResourceViews_[MAX_TEXTURE_UNITS];

		SamplerState* samplerStates[MAX_TEXTURE_UNITS];
		unsigned firstDirtySamplerState_;
		unsigned lastDirtySamplerState_;
		bool	samplerStatesDirty_;
		ID3D11SamplerState* samplers_[MAX_TEXTURE_UNITS];
		map<SamplerState, ID3D11SamplerState*> SamplerStatesAll;

		ID3D11Buffer* constantBuffers_[2][MAX_SHADER_PARAMETER_GROUPS];

	};

	ID3D11Device* GetD3D11DxDevive();
	ID3D11DeviceContext* GetD3D11DxDeviveContext();
}


