#ifndef  _GLESRENDERDevice__H__
#define  _GLESRENDERDevice__H__

#include "Engine/RenderSystem/IRenderDevice.h"

namespace ma
{
	class IndexBuffer;
	class VertexBuffer;
	class RenderTarget;
	class GLESTechnique;

	class GLESRENDER_API GLESRenderDevice : public IRenderDevice
	{
	public:
		GLESRenderDevice();

		~GLESRenderDevice();

		/// Vido Buffer 
		virtual Texture*			CreateRendTexture();

		virtual VertexDeclaration*	CreateVertexDeclaration();

		virtual VertexBuffer*		CreateVertexBuffer(void* pData = NULL, int nsize = 0, int nStride = 0, USAGE Usgae = USAGE_STATIC);

		virtual IndexBuffer*		CreateIndexBuffer(void* Data = NULL, int size = 0, INDEX_TYPE eIndexType = INDEX_TYPE_U16, USAGE Usgae = USAGE_STATIC);

		virtual	Technique*			CreateTechnique();
		

		//// Render
		virtual void			SetRenderTarget(int index,Texture* pTexture);
		
		//virtual void			SetVertexStream(VertexStream* pVertexStream);

		//virtual void			DrawIndexedPrimitive(PRIMITIVE_TYPE ePrimType,UINT startVertex,UINT vertexCount,UINT startIndex,UINT IndexCount);
		virtual	void			DrawRenderMesh(RenderMesh* pRenderMesh,Technique* pTech);

		virtual	void			DrawRenderable(Renderable* pRenderable);

		virtual void			DrawMeshBatch(MeshBatch* pMeshBatch,Technique* pTech);

		virtual	void			ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s);
		
		virtual void			DrawLine(const Vector3& p0,const Vector3& p1,Uint32 dwColor);


		/// Render Pass
		virtual	void			Init(HWND wndhandle);

		virtual void			BeginRender();
	
		virtual void			EndRender();

		/// Util
		virtual	void			GetRenderWndSize(int& Width,int& Heigh);
		
		virtual	Matrix4x4		MakeProjectionMatrix(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf);

	private:
#if PLATFORM_WIN == 1
		HDC		m_hDC;
		HGLRC	m_hGLRC;
#endif
	};
}

#endif


