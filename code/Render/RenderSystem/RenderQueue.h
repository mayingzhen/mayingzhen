#ifndef  _RenderQueue__H__
#define  _RenderQueue__H__



namespace ma
{
	class RenderObject;

	enum RenderListType
	{
		RL_Solid,
		RL_Trans,
		RL_Count,
	};

	class  RenderQueue
	{
	public:
		void			AddRenderObj(RenderListType eRLType,RenderObject* pRenderObj);	

		UINT			GetRenderObjNumber(RenderListType eRLType);

		RenderObject*	GetRenderObjByIndex(RenderListType eRLType,int index);

		void			Clear();

	private:

		std::vector<RenderObject*>		m_arrRenderList[RL_Count];
	};
}

#endif // _RenderQueue__H__
