#ifndef  _RendererObject__H__
#define  _RendererObject__H__


namespace ma
{
	class Material;
	class Material;

	class RENDER_API RenderObject : public Component
	{
		DECL_OBJECT(RenderObject)

	public:
		RenderObject(GameObject* pGameObj);

		virtual	void		UpdateTransform();

		virtual void		Render(Technique* pTech) = 0;

		virtual	void		AddToRenderQueue() = 0;
	
		virtual Material*	GetMaterial() = 0;

		virtual void		SetMaterial(Material* pMaterial) = 0;

		//virtual AABB		GetAABBWS() = 0;

		virtual void		SetWorldMatrix(const Matrix4x4& matWS) = 0;
	};
}

#endif // _RendererObject__H__
