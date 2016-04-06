#ifndef  _RenderPassPass__H__
#define  _RenderPassPass__H__

namespace ma
{
	class Scene;

	class RenderPass : public Object
	{
	public:
		RenderPass(Scene* pScene)
		{
			m_pScene = pScene;
		}

		virtual void Init() = 0;

		virtual void Reset() = 0;

		virtual void Render() = 0;

		virtual void Shoutdown() = 0;

	protected:
		Scene*	m_pScene;
	};
}

#endif
