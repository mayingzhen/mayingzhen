#ifndef  _RenderQueue__H__
#define  _RenderQueue__H__



namespace ma
{
	class RenderComponent;

	enum RenderListType
	{
		RL_Solid,
		RL_Trans,
		RL_TerrainBody,
		RL_TerrainBorder,
		RL_TerrainSkirt,
		RL_Count,
	};

	class RenderQueue
	{
	public:
		void					AddRenderObj(RenderListType eRLType,Renderable* pRenderObj);	
		
		void					SortRenderObjList(RenderListType eRLType);

		void					RenderObjList(RenderListType eRLType);

		void					Clear();

	private:
		UINT					GetRenderObjNumber(RenderListType eRLType);

		Renderable*				GetRenderObjByIndex(RenderListType eRLType,int index);
	
	private:
		typedef std::vector< RefPtr<Renderable> > VEC_RENDERABLE;
		VEC_RENDERABLE	m_arrRenderList[RL_Count];
	};
}

#endif // _RenderQueue__H__
