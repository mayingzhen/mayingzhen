#ifndef  _RenderQueue__H__
#define  _RenderQueue__H__

namespace ma
{
	class IRenderItem;

	class ENGINE_API RenderQueue
	{
	public:
		RenderQueue();
		
		virtual ~RenderQueue();

		virtual void	AddRenderer(IRenderItem * obj);

		//void SortTransparency(Camera * cam);
		void			Clear();

		void			Flush();

		//const Array<Renderer *> & GetSolidRender() { return mSolidEntry; }
		//const Array<Renderer *> & GetTransRender() { return mTransEntry; }
		UINT			GetSolidEntryNumber() {return m_SolidEntry.size();}

		IRenderItem*	GetSolidEntryByIndex(UINT uIndex) {return m_SolidEntry[uIndex];}

	protected:
		//void _pushRenderer(SceneNode * node);

	protected:
		std::vector<IRenderItem*> m_SolidEntry;
		std::vector<IRenderItem*> m_TransEntry;
	};

	ENGINE_API RenderQueue*	GetRenderQueue();
	
	ENGINE_API void			SetRenderQueue(RenderQueue* pRenderQueue);
}

#endif
