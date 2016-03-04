#ifndef  _RenderQueue__H__
#define  _RenderQueue__H__



namespace ma
{
	class BatchRenderable;

	enum RenderListType
	{
		RL_Mesh,
		RL_MeshTrans,
		RL_Particle,
		RL_Terrain,
		RL_Count,
	};

	class RenderQueue
	{
	public:
		RenderQueue();

		~RenderQueue();

		void					AddRenderObj(RenderListType eRLType,Renderable* pRenderObj);	

		void					RenderObjList(RenderListType eRLType);

		void					Clear();

	private:
		UINT					GetRenderObjNumber(RenderListType eRLType);

		Renderable*				GetRenderObjByIndex(RenderListType eRLType,int index);
	
	private:
		BatchRenderable*		m_arrRenderList[RL_Count];
	};
}

#endif // _RenderQueue__H__
