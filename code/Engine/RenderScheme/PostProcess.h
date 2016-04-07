#ifndef _PostProcess__H__
#define _PostProcess__H__

namespace ma
{
	class PostProcess : public Object
	{
	public:
		virtual void Init() = 0; 

		virtual void Reset(Texture* pInput, Texture* pOutput) = 0;

		virtual void Shutdown() = 0;

		virtual void Render() = 0;

	protected:

		RefPtr<Texture>	m_pInputTex;

		RefPtr<Texture> m_pOutputTex;
	};

}


#endif
