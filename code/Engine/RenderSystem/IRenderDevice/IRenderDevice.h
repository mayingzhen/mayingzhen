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
	class RenderState;
	class Technique;
	class Uniform;
	class FrameBuffer;

	enum RenderDeviceType
	{
		RenderDevice_NULL,
		RenderDevice_D3D11,
		RenderDevice_GLES2
	};

	class IRenderDevice 
	{
	private:
		virtual	void				SetFrameBuffer(FrameBuffer* pFB) = 0;
		virtual	void				SetRenderTarget(Texture* pTexture,int index = 0) = 0;
		virtual	Texture*			GetDefaultRenderTarget(int index = 0) = 0;
		virtual	void				SetDepthStencil(Texture* pTexture) = 0;
		virtual	Texture*			GetDefaultDepthStencil() = 0;
		virtual void				SetViewport(const Rectangle& rect) = 0;
		virtual Rectangle			GetViewport() = 0;

		virtual	void				SetDepthBias(float constantBias, float slopeScaleBias = 0.0f) = 0;
		virtual	void				SetCullingMode(CULL_MODE mode) = 0;
		virtual void				SetBlendMode(BLEND_MODE mode) = 0;
		virtual	void				SetDepthCheckMode(CompareFunction mode) = 0;
		virtual	void				SetDepthWrite(bool b) = 0;
		virtual	void				SetColorWrite(bool b) = 0;
		virtual	void				SetSRGBWrite(bool b) = 0;
		virtual void				SetStencilEnable(bool b) = 0;
		virtual void				SetStencilBufferParams(CompareFunction func = CMPF_ALWAYS_PASS, 
			uint32 refValue = 0, uint32 mask = 0xFFFFFFFF, uint32 writeMask = 0xFFFFffff,
			StencilOperation stencilFailOp = SOP_KEEP, 
			StencilOperation depthFailOp = SOP_KEEP,
			StencilOperation passOp = SOP_KEEP, 
			bool twoSidedOperation = false) = 0;
		
		virtual void				SetValue(Uniform* uniform, float value) = 0;
		virtual void				SetValue(Uniform* uniform, const Vector2& value) = 0;
		virtual void				SetValue(Uniform* uniform, const Vector3& value) = 0;
		virtual void				SetValue(Uniform* uniform, const Vector4* values, UINT count) = 0;
		virtual void				SetValue(Uniform* uniform, const Matrix4* values, UINT count) = 0;
		virtual void				SetValue(Uniform* uniform, const ColourValue& value) = 0;
		virtual	void				SetTexture(Uniform* uniform,Texture* pTexture) = 0;
		virtual	void				SetTexture(uint32 nIndex,Texture* pTexture) = 0;
		virtual	void				SetSamplerState(Uniform* uniform,SamplerState* pSampler) = 0;
		
		virtual	void				SetVertexDeclaration(VertexDeclaration* pDec) = 0;
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
		virtual Texture*			CreateRenderTarget(int nWidth,int nHeight,PixelFormat format,bool bTypeLess,bool bSRGB) = 0;
		virtual Texture*			CreateDepthStencil(int nWidth,int nHeight,PixelFormat format,bool bTypeLess) = 0;
		virtual VertexDeclaration*	CreateVertexDeclaration() = 0;
		virtual VertexBuffer*		CreateVertexBuffer() = 0;
		virtual IndexBuffer*		CreateIndexBuffer() = 0;
		virtual ShaderProgram*		CreateShaderProgram() = 0;

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