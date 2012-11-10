#ifndef  _DXRENDWINDOW__H__
#define  _DXRENDWINDOW__H__

namespace ma
{
	class DXRENDER_API DxRendWindow : public IRendWindow
	{
	public:
		DxRendWindow();

		~DxRendWindow();

		void Init();

		void Shutdown();

		void BeginRender();

		void EndRender();

	private:
		HWND m_hWnd;

		IDirect3DSwapChain9* m_pSwapChain;
		IDirect3DSurface9* m_pBackuffer;
	};
}




#endif