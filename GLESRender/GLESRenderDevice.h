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

		virtual VertexBuffer*		CreateVertexBuffer();

		virtual IndexBuffer*		CreateIndexBuffer();

		virtual	Technique*			CreateTechnique();
		

		//// Render
		virtual void			SetRenderTarget(int index,Texture* pTexture);
		
		//virtual void			SetVertexStream(VertexStream* pVertexStream);

		//virtual void			DrawIndexedPrimitive(PRIMITIVE_TYPE ePrimType,UINT startVertex,UINT vertexCount,UINT startIndex,UINT IndexCount);
		virtual	void			DrawRenderMesh(RenderMesh* pRenderMesh,Technique* pTech);

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
		HDC		m_hDC;
		HGLRC	m_hGLRC;
	};
}

#endif


