#ifndef  _IRenderDevice__H__
#define  _IRenderDevice__H__

#include "Engine/ImageData/PixelFormat.h"

namespace ma
{
	class Texture;
	class SamplerState;
	class VertexBuffer;
	class IndexBuffer;
	class VertexDeclaration;
	class Camera;
	class Light;
	class VertexStream;
	class MeshComponent; 
	class MeshBatch;
	class Renderable;
	class ShaderProgram;
	class BlendState;
	class DepthStencilState;
	class RasterizerState;
	class Technique;
	class Uniform;
	class FrameBuffer;

	enum RenderDeviceType
	{
		RenderDevice_NULL,
		RenderDevice_D3D11,
		RenderDevice_GLES2,
		RenderDevice_VULKAN,
        RenderDevice_METAL
	};

	class IRenderDevice 
	{
	private:
		virtual	void				SetFrameBuffer(FrameBuffer* pFB) = 0;
		virtual	void				SetRenderTarget(int index,Texture* pTexture,int level = 0, int array_index = 0, int face = 0) = 0;
		virtual	Texture*			GetDefaultRenderTarget(int index = 0) = 0;
		virtual	void				SetDepthStencil(Texture* pTexture) = 0;
		virtual	Texture*			GetDefaultDepthStencil() = 0;
		virtual void				SetViewport(const Rectangle& rect) = 0;
		virtual Rectangle			GetViewport() = 0;

		virtual void				SetBlendState(const BlendState* pBlendState) = 0;
		virtual void				SetDepthStencilState(const DepthStencilState* pDSState,UINT nStencilRef) = 0;
		virtual void				SetRasterizerState(const RasterizerState* pRSState) = 0;
		
		virtual void				SetValue(Uniform* uniform, int value) = 0;
		virtual void				SetValue(Uniform* uniform, float value) = 0;
		virtual void				SetValue(Uniform* uniform, const Vector2& value) = 0;
		virtual void				SetValue(Uniform* uniform, const Vector3& value) = 0;
		virtual void				SetValue(Uniform* uniform, const Vector4* values, UINT count) = 0;
		virtual void				SetValue(Uniform* uniform, const Matrix4* values, UINT count) = 0;
		virtual void				SetValue(Uniform* uniform, const ColourValue& value) = 0;
		virtual	void				SetTexture(Uniform* uniform,Texture* pTexture) = 0;
		virtual	void				SetTexture(uint32 nIndex,Texture* pTexture,bool bSRGBNotEqual) = 0;
		virtual	void				SetSamplerState(Uniform* uniform,SamplerState* pSampler) = 0;
		
		virtual	void				SetVertexDeclaration(const VertexDeclaration* pDec) = 0;
		virtual void				SetIndexBuffer(IndexBuffer* pIB) = 0;
		virtual	void				SetVertexBuffer(int index, VertexBuffer* pVB) = 0;

		virtual void				DrawRenderable(const Renderable* pRenderable,Technique* pTech) = 0;
		virtual	void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s) = 0;

		virtual	void				Init(HWND wndhandle) = 0;
		virtual void				Shoutdown() = 0;

		virtual bool				TestDeviceLost() = 0;	
		virtual void				BeginRender() = 0;
		virtual void				EndRender() = 0;

		virtual	void				BeginProfile(const char* pszLale) = 0;
		virtual	void				EndProfile() = 0;
		
	public:
		virtual	RenderDeviceType	GetRenderDeviceType() = 0;

		virtual Texture*			CreateTexture() = 0;
		virtual Texture*			CreateRenderTarget(int nWidth,int nHeight,UINT32 nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType) = 0;
		virtual Texture*			CreateDepthStencil(int nWidth,int nHeight,PixelFormat format,bool bTypeLess) = 0;
		virtual VertexDeclaration*	CreateVertexDeclaration() = 0;
		virtual VertexBuffer*		CreateVertexBuffer() = 0;
		virtual IndexBuffer*		CreateIndexBuffer() = 0;
		virtual ShaderProgram*		CreateShaderProgram() = 0;

		virtual BlendState*			CreateBlendState() = 0;
		virtual DepthStencilState*	CreateDepthStencilState() = 0;
		virtual RasterizerState*	CreateRasterizerState() = 0;

		virtual SamplerState*		CreateSamplerState() = 0;

		virtual	Matrix4				MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf) = 0;
		virtual	Matrix4				MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf) = 0;		
		virtual Matrix4				MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf) = 0;

		//CheckDevice
		virtual	bool				CheckTextureFormat(PixelFormat eFormat,TEXTURE_USAGE eUsage) = 0;

		friend class RenderSystem;
		friend class RenderThread;
	};

	 void SetRenderDevice(IRenderDevice* pRenderDevice);
	 IRenderDevice* GetRenderDevice();
}


#endif
