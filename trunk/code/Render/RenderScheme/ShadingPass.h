#ifndef  _ShadingPass__H__
#define  _ShadingPass__H__

namespace ma
{
	class ShadingPass : public RenderPass
	{
	public:
		void Init();

		void Render();

		void ShoutDown();
	};

}


#endif
