#ifndef _PostProcess__H__
#define _PostProcess__H__

namespace ma
{
	class PostProcess 
	{
	public:
		PostProcess(Texture* pInputTex, Texture* pOutputTex)
		{
			m_pInputTex = pInputTex;
			m_pOutputTex = pOutputTex;
		}

		virtual void Init() = 0; 

		virtual void Shutdown() = 0;

		virtual void Render() = 0;

	protected:

		Texture*		m_pInputTex;

		Texture*		m_pOutputTex;
	};

}


#endif
