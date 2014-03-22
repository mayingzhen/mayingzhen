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
	class MeshComponent; 
	class MeshBatch;
	struct Renderable;
	class ShaderProgram;
	struct RenderState;
	class Technique;

	class RENDER_API IRenderDevice 
	{
	public:
		virtual Texture*			CreateTexture(const char* pszPath = NULL) = 0;

		virtual Texture*			CreateTexture(int nWidth,int nHeight,FORMAT format = FMT_A8R8G8B8,bool bDepthStencil = false) = 0;

		virtual VertexDeclaration*	CreateVertexDeclaration() = 0;

		virtual VertexBuffer*		CreateVertexBuffer() = 0;

		virtual IndexBuffer*		CreateIndexBuffer() = 0;
	
		virtual ShaderProgram*		CreateShaderProgram(Technique* pTech,const char* pVSFile,const char* pPSFile,const char* pszDefine) = 0;

		virtual	void				SetRenderTarget(Texture* pTexture,int index = 0) = 0;

		virtual	Texture*			GetRenderTarget(int index = 0) = 0;
		
		virtual	void				SetDepthStencil(Texture* pTexture) = 0;

		virtual	Texture*			GetDepthStencil() = 0;

		virtual void				SetViewport(const Rectangle& rect) = 0;

		virtual Rectangle			GetViewport() = 0;

		virtual void				SetRenderState(const RenderState& state) = 0;

		virtual void				DrawRenderable(const Renderable* pRenderable,Technique* pTech) = 0;

		virtual void				DrawDyRenderable(const Renderable* pRenderable,Technique* pTech) = 0;
	
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
	
		virtual	void				GetProjectionNearFar(float& fProjNear, float& fProjFar) = 0;

		virtual	void				BeginProfile(const char* pszLale) = 0;

		virtual	void				EndProfile() = 0;

		//CheckDevice
		virtual	bool				CheckTextureFormat(FORMAT eFormat,USAGE eUsage) = 0;

	};

	RENDER_API void SetRenderDevice(IRenderDevice* pRenderDevice);

	RENDER_API IRenderDevice* GetRenderDevice();
}


#endif