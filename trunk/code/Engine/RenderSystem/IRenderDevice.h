#ifndef  _IRenderDevice__H__
#define  _IRenderDevice__H__

namespace ma
{
	class IRenderItem;
	class Texture;
	class VertexBuffer;
	class IndexBuffer;
	class VertexDeclaration;
	class Camera;
	class Light;
	class VertexStream;
	class Technique;
	class RenderMesh; 
	class MeshBatch;
	class Renderable;
	class ShaderProgram;
	class RenderState;

	//typedef void* Technique;

	class ENGINE_API IRenderDevice
	{
	public:

		/// Vido Buffer
		virtual Texture*			CreateRendTexture() = 0;

		virtual VertexDeclaration*	CreateVertexDeclaration() = 0;

		virtual VertexBuffer*		CreateVertexBuffer(void* pData = NULL, int nsize = 0, int nStride = 0, USAGE Usgae = USAGE_STATIC) = 0;

		virtual IndexBuffer*		CreateIndexBuffer(void* Data = NULL, int size = 0, INDEX_TYPE eIndexType = INDEX_TYPE_U16, USAGE Usgae = USAGE_STATIC) = 0;
	
		virtual ShaderProgram*				CreateEffect() = 0;

		/// Render
		virtual void				SetRenderTarget(int index,Texture* pTexture) = 0;

		virtual void				SetRenderState(const RenderState& state) = 0;

		//virtual void				SetVertexStream(VertexStream* pVertexStream) = 0;

		//virtual void				DrawIndexedPrimitive(PRIMITIVE_TYPE ePrimType,UINT startVertex,UINT vertexCount,UINT startIndex,UINT IndexCount) = 0;
		
		virtual	void				DrawRenderMesh(RenderMesh* pRenderMesh,Technique* pTech) = 0;

		virtual	void				DrawRenderable(Renderable* pRenderable) = 0;
		
		virtual void				DrawMeshBatch(MeshBatch* pMeshBatch,Technique* pTech) = 0;

		virtual	void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s) = 0;
	
		virtual void				DrawLine(const Vector3& p0,const Vector3& p1,Uint32 dwColor) = 0;


		/// Render pass
		virtual	void				Init(HWND wndhandle) = 0;

		virtual void				BeginRender() = 0;

		virtual void				EndRender() = 0;

		virtual void				SetCamera(Camera* pCamera)  {}

		virtual Camera*				GetCamera() {return NULL;}

		virtual void				ClearLightList() {}

		virtual void				AddLight(Light* pLigt) {}

		virtual	void				GetRenderWndSize(int& Width,int& Heigh) = 0;

		virtual	Matrix4x4			MakeProjectionMatrix(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf) = 0;


		/// Util
		virtual void				DrawBox(const Matrix4x4& wordMat,const Vector3& boxSize, Uint32 color);

		virtual void				DrawCircle(UINT nbSegments, const Matrix4x4& world, Uint32 dwColor, float radius, bool semicircle = false);

		virtual void				DrawWireSphere(const Matrix4x4& wordMat,float fRadius, Uint32 color);
	};

	ENGINE_API void SetRenderDevice(IRenderDevice* pRenderDevice);

	ENGINE_API IRenderDevice* GetRenderDevice();
}


#endif