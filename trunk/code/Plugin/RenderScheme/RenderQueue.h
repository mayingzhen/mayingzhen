#ifndef  _RenderQueue__H__
#define  _RenderQueue__H__



namespace ma
{
	class RENDERSCHEME_API RenderQueue
	{
	public:
		static void AddRenderable(Renderable* pRenderable,bool bTrans = false);
		
		static void Clear();

	protected:
		static std::vector<Renderable*>   m_SolidEntry;
		static std::vector<Renderable*>   m_TransEntry;
	};

}

#endif