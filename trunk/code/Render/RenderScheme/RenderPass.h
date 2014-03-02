#ifndef  _RenderPassPass__H__
#define  _RenderPassPass__H__

namespace ma
{
	class RENDER_API RenderPass : public Object
	{
		DECL_OBJECT(RenderPass)

	public:
		virtual void Init() = 0;

		virtual void Render() = 0;

		virtual void ShoutDown() = 0;
	};
}

#endif
