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

		//virtual void RenderSkelMesh(const D3DXMATRIX* arrSkinMatrix,UINT nSkinMaxtrixNum,
		//	const D3DXMATRIX* pWordMat,const IRendMesh* pSkelMesh,const IRendTexture* pTexture) = 0;

		virtual void			DrawLine(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,xmUint32 dwColor) = 0;

		virtual void			DrawBox(const D3DXMATRIX& wordMat,const D3DXVECTOR3& boxSize, xmUint32 color);

		virtual void			DrawCircle(UINT nbSegments, const D3DXMATRIX& world, xmUint32 dwColor, float radius, bool semicircle = false);

		virtual void			DrawWireSphere(const D3DXMATRIX& wordMat,float fRadius, xmUint32 color);
	};

	ENGINE_API void SetRenderDevice(IRenderDevice* pRenderDevice);

	ENGINE_API IRenderDevice* GetRenderDevice();
}


#endif