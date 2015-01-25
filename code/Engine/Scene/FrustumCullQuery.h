#ifndef  _FrustumCullQuery__H__
#define  _FrustumCullQuery__H__

namespace ma
{
	class FrustumCullQuery : public CullTreeQuery
	{
	public:
		FrustumCullQuery(const Frustum& cameraFrustum, std::vector< RefPtr<RenderComponent> >& result);

		Frustum::Visibility TestCullNode(CullNode* pCullNode);

		void VisitCullNode(CullNode* pCullNode,Frustum::Visibility eNodeVisib);


	public:
		const Frustum&			m_cameraFrustum;

		typedef std::vector< RefPtr<RenderComponent> > VEC_RENDERCOMP;
		VEC_RENDERCOMP&			m_result;
	};
}

#endif
