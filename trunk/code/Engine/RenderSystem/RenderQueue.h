#ifndef  _RenderQueue__H__
#define  _RenderQueue__H__



namespace ma
{
	class RenderComponent;

	enum RenderListType
	{
		RL_Solid,
		RL_Trans,
		RL_Count,
	};

	class  RenderQueue
	{
	public:
		void					AddRenderObj(RenderListType eRLType,RenderablePtr pRenderObj);	

		UINT					GetRenderObjNumber(RenderListType eRLType);

		Renderable*				GetRenderObjByIndex(RenderListType eRLType,int index);

		void					Clear();

	private:
		std::vector<RenderablePtr>	m_arrRenderList[RL_Count];
	};
}

#endif // _RenderQueue__H__