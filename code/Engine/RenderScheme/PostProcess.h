#ifndef _PostProcess__H__
#define _PostProcess__H__

namespace ma
{
	class PostProcess : public RenderPass
	{
	public:
		PostProcess(Scene* pScene):RenderPass(pScene)
		{
		}

		virtual void Init() = 0; 

		virtual void Reset() = 0;

		virtual void Shutdown() = 0;

		virtual void Render() = 0;

	protected:

		RefPtr<Texture>	m_pInputTex;


		RefPtr<Texture> m_pOutputTex;
	};

}


#endif
