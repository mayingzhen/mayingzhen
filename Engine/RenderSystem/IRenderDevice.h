#ifndef  _IRenderDevice__H__
#define  _IRenderDevice__H__

namespace ma
{
	class IRendMesh;
	class IRendTexture;
	class Texture;
	class Camera;
	class Light;

	class ENGINE_API IRenderDevice
	{
	public:
		virtual	void			Init(int wndhandle) = 0;
		
		virtual void			BeginRender() = 0;

		virtual void			EndRender() = 0;
	
		virtual IRendMesh*		CreateRendMesh() = 0;

		virtual IRendTexture*	CreateRendTexture() = 0;

		virtual void			SetCamera(Camera* pCamera) = 0;

		virtual Camera*			GetCamera() = 0;

		virtual void			ClearLightList() = 0;

		virtual void			AddLight(Light* pLigt) = 0;

		virtual void			RenderMesh(IRendMesh* pMesh) = 0;

		virtual	void			GetRenderWndSize(int& Width,int& Heigh) = 0;

		virtual	Matrix4x4		MakeProjectionMatrix(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf) = 0;

		//virtual void RenderSkelMesh(const Matrix4x4* arrSkinMatrix,UINT nSkinMaxtrixNum,
		//	const Matrix4x4* pWordMat,const IRendMesh* pSkelMesh,const IRendTexture* pTexture) = 0;

		virtual void			DrawLine(const Vector3& p0,const Vector3& p1,Uint32 dwColor) = 0;

		virtual void			DrawBox(const Matrix4x4& wordMat,const Vector3& boxSize, Uint32 color);

		virtual void			DrawCircle(UINT nbSegments, const Matrix4x4& world, Uint32 dwColor, float radius, bool semicircle = false);

		virtual void			DrawWireSphere(const Matrix4x4& wordMat,float fRadius, Uint32 color);
	};

	ENGINE_API void SetRenderDevice(IRenderDevice* pRenderDevice);

	ENGINE_API IRenderDevice* GetRenderDevice();
}


#endif