#pragma once

namespace ma
{
	class BatchRenderable;
	class Renderable;
	class RenderPass;

	enum RenderListType
	{
		RL_Mesh,
		RL_Terrain,
		RL_SkyBox,
		RL_MeshTrans,
		RL_Count,
	};

	enum RenderPassType
	{
		RP_ShadowDepth,
		RP_Shading,
		RP_Count,
	};

	class RenderQueue
	{
	public:
		RenderQueue();

		~RenderQueue();

		void					AddRenderObj(RenderListType eRLType,Renderable* pRenderObj);	

		void					RenderObjList(RenderPass* pPass, RenderListType eRLType, RenderPassType eRPType);

		void					Clear();
	
	private:
		BatchRenderable*		m_arrRenderList[RL_Count];
	};
}
