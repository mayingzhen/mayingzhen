#ifndef  _IRenderDevice__H__
#define  _IRenderDevice__H__

namespace ma
{
	class Texture;
	class VertexBuffer;
	class IndexBuffer;
	class VertexDeclaration;
	class Camera;
	class Light;
	class VertexStream;
	class RenderMesh; 
	class MeshBatch;
	struct Renderable;
	class ShaderProgram;
	struct RenderState;
	class RenderTarget;
	class Technique;

	class ENGINE_API IRenderDevice 
	{
	public:
		virtual Texture*			CreateTexture(const char* pszPath = NULL) = 0;

		virtual Texture*			CreateTexture(int nWidth,int nHeight,FORMAT format) = 0;

		virtual RenderTarget*		CreateRenderTarget(int nWidth = -1,int nHeight = -1,FORMAT format = FMT_A8R8G8B8) = 0;

		virtual VertexDeclaration*	CreateVertexDeclaration() = 0;

		virtual VertexBuffer*		CreateVertexBuffer(void* pData = NULL, int nsize = 0, int nStride = 0, USAGE Usgae = USAGE_STATIC) = 0;

		virtual IndexBuffer*		CreateIndexBuffer(void* Data = NULL, int size = 0, INDEX_TYPE eIndexType = INDEX_TYPE_U16, USAGE Usgae = USAGE_STATIC) = 0;
	
		virtual ShaderProgram*		CreateShaderProgram(const char* pszName,const char* pszDefine) = 0;

		virtual	void				PushRenderTarget(RenderTarget* pTexture,int index = 0) = 0;

		virtual void				PopRenderTarget(int index = 0) = 0;
		
		virtual	void				PushDepthStencil(RenderTarget* pTexture) = 0;

		virtual void				PopDepthStencil() = 0;

		virtual void				PushViewport(const Rectangle& rect) = 0;

		virtual void				PopViewport() = 0;

		virtual void				SetRenderState(const RenderState& state) = 0;

		virtual void				DrawRenderable(const Renderable* pRenderable) = 0;

		virtual void				DrawDyRenderable(const Renderable* pRenderable) = 0;
	
		virtual	void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s) = 0;

		/// Render pass
		virtual	void				Init(HWND wndhandle) = 0;

		virtual void				BeginRender() = 0;

		virtual void				EndRender() = 0;


		// util 
		virtual const char*			GetShaderPath() = 0;

		virtual void				ConvertUV(float& fTop,float& fLeft,float& fRight,float& fBottom) = 0;

		virtual float				GetHalfPixelOffset(float fHalfPiexl) = 0;

		virtual	Matrix4x4			MakePerspectiveMatrix(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf) = 0;

		virtual	Matrix4x4			MakeOrthoMatrix(Matrix4x4 *pOut, float width, float height, float zn, float zf) = 0;
		
		virtual Matrix4x4			MakeOrthoMatrixOffCenter(Matrix4x4 *pOut, float left, float right, float bottom, float top, float zn, float zf) = 0;
	};

	ENGINE_API void SetRenderDevice(IRenderDevice* pRenderDevice);

	ENGINE_API IRenderDevice* GetRenderDevice();
}


#endif