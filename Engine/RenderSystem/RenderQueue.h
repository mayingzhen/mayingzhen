#ifndef  _RenderQueue__H__
#define  _RenderQueue__H__



namespace ma
{

	class Renderer;
	class SceneNode;

	class ENGINE_API RenderQueue
	{
	public:
		RenderQueue();
		virtual ~RenderQueue();

		void PushRenderer(const std::list<SceneNode *> & nodes);
		virtual void AddRenderer(Renderer * obj);

		void SortTransparency(Camera * cam);
		void Clear();

		//const Array<Renderer *> & GetSolidRender() { return mSolidEntry; }
		//const Array<Renderer *> & GetTransRender() { return mTransEntry; }

	protected:
		void _pushRenderer(SceneNode * node);

	protected:
		std::vector<IRenderItem*>   mSolidEntry;
		std::vector<IRenderItem*>   mTransEntry;
	};

}