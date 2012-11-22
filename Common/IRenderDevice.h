#ifndef  _IRenderDevice__H__
#define  _IRenderDevice__H__

namespace ma
{
	class IRendMesh;
	class IRendTexture;

	class COMMON_API IRenderDevice
	{
	public:
		virtual IRendMesh* CreateRendMesh() = 0;

		virtual IRendTexture* CreateRendTexture() = 0;
	};
}


#endif