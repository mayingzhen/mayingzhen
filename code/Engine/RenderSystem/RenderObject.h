#ifndef  _RendererObject__H__
#define  _RendererObject__H__


namespace ma
{
	class RenderObject
	{
	public:
		virtual void		Render() = 0;
	
		virtual Material*	GetMaterial() = 0;
	};
}

#endif // _RendererObject__H__