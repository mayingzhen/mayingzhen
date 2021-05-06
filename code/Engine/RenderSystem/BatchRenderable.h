#pragma once

namespace ma
{
	class InstanceRenderable;
	class Renderable;
	class Technique;

	class BatchRenderable
	{
	public:
		BatchRenderable();

		virtual void	AddRenderObj(Renderable* pRenderObj, Technique* pTech);

		virtual void	PrepareRender(RenderQueue* pRenderQueue);

		virtual void	Clear();
		
	private:

		struct InstanceKey
		{
			RefPtr<Technique> m_pTech;
			RefPtr<IndexBuffer>	m_pIndexBuffer;
			RefPtr<VertexBuffer> m_pVertexBuffer;

			bool operator < (const InstanceKey& other) const
			{
				#define CMPVAR(x) if (x != other.x) return x < other.x;
				CMPVAR(m_pTech);
				CMPVAR(m_pIndexBuffer);
				CMPVAR(m_pVertexBuffer);
				#undef CMPVAR

				return false;
			}
		};

		typedef std::map<InstanceKey, RefPtr<InstanceRenderable> > MAP_RENDERLIST;
		MAP_RENDERLIST m_mapInsRenderList;

		RefPtr<VertexBuffer> m_pVertexBuffer[3];
		uint32_t m_nBufferIndex = 0;
	};
}

