#ifndef  _MeshRendererable__H__
#define  _MeshRendererable__H__

#include "Renderable.h"

namespace ma
{
	class MeshRenderable : public Renderable
	{
	public:
		MeshRenderable();

		virtual void					PreRender(Technique* pTech);
		virtual void					Render(Technique* pTech);
        
    public:
        
        //Vector3 m_pos_extent;
        //Vector3 m_pos_center;
        //Vector4 m_tc_extent_center;
    };
}

#endif

