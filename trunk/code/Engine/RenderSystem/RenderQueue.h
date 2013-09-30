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

	class ENGINE_API  RenderQueue
	{
	public:
		void			AddRenderObj(RenderListType eRLType,RenderObject* pRenderObj);	

		UINT			GetRenderObjNumber(RenderListType eRLType);

		RenderObject*	GetRenderObjByIndex(RenderListType eRLType,int index);

		void			Clear();

	private:

		std::vector<RenderObject*>		m_arrRenderList[2][RL_Count];
	};

	ENGINE_API RenderQueue*	GetRenderQueue();

	ENGINE_API void			SetRenderQueue(RenderQueue* pRenderQueue);

}

#endif // _RenderQueue__H__
