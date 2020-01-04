#ifndef  _IRenderDevice__H__
#define  _IRenderDevice__H__

#include "Engine/ImageData/PixelFormat.h"
#include "../RenderQueue.h"

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
	class RenderPass;
	class ConstantBuffer;
	class RenderCommand;
	class ComputeCommad;

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

		virtual Rectangle			GetViewport() = 0;
		virtual RenderPass*			GetBackBufferRenderPass() = 0;

		virtual	void				Init(void* wndhandle) = 0;
		virtual void				Shoutdown() = 0;

		virtual void				BeginRender() = 0;
		virtual void				EndRender() = 0;

		virtual void				BegineCompute() = 0;
		virtual void				EndCompute() = 0;

		virtual	void				BeginProfile(const char* pszLale) = 0;
		virtual	void				EndProfile() = 0;

	public:
		virtual	RenderDeviceType	GetRenderDeviceType() = 0;

		virtual Texture*			CreateTexture() = 0;
		virtual Texture*			CreateRenderTarget(int nWidth,int nHeight,uint32_t nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType) = 0;
		virtual Texture*			CreateDepthStencil(int nWidth,int nHeight,PixelFormat format) = 0;
		virtual VertexDeclaration*	CreateVertexDeclaration() = 0;
		virtual VertexBuffer*		CreateVertexBuffer() = 0;
		virtual IndexBuffer*		CreateIndexBuffer() = 0;
		virtual ShaderProgram*		CreateShaderProgram() = 0;
		virtual ConstantBuffer*		CreateConstantBuffer(uint32_t nSize) = 0;
		virtual BlendState*			CreateBlendState() = 0;
		virtual DepthStencilState*	CreateDepthStencilState() = 0;
		virtual RasterizerState*	CreateRasterizerState() = 0;
		virtual SamplerState*		CreateSamplerState() = 0;
		virtual Technique*			CreateTechnique() = 0;
		virtual RenderPass*			CreateRenderPass() = 0;
		virtual RenderCommand*		CreateRenderCommand() = 0;
		virtual ComputeCommad*		CreateComputeCommand() = 0;

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
