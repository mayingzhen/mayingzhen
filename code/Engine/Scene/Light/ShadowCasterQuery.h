#pragma once

#include "../../Scene/CullTree.h"

namespace ma
{

class ShadowCasterQuery : public CullTreeQuery
{
public:
	ShadowCasterQuery(const Frustum& lightFrustum,const Frustum& lightViewFrustum, 
		const Matrix4& matLightView, vector<RenderComponent*>& result, AABB& castrAABB);

	Frustum::Visibility TestCullNode(CullNode* pCullNode);

	void VisitCullNode(CullNode* pCullNode,Frustum::Visibility eNodeVisib);

public:
	typedef vector<RenderComponent*> VEC_RESULT;

	const Frustum&	m_lightFrustum;

	const Frustum& m_lightViwFrustum;

	const Matrix4&	m_matLightView;

	VEC_RESULT		m_result;

	AABB&			m_casteAAbb;
};

}