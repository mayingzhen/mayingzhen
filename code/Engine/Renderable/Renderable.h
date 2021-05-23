#pragma once

namespace ma
{
	class SubMaterial;
	class Technique;
	class MeshData;
	class SubMeshData;
	class SceneContext;

	class Renderable : public Referenced
	{
	public:
		Renderable();

		virtual void					PreRender(Technique* pTech, SceneContext* sc);

		virtual void					Render(Technique* pTechnique, RenderCommand* pRenderCommand);

		virtual void					Compute(Technique* pTechnique, ComputeCommand* pComputeCmd, SceneContext* sc);

		void							SetWorldMatrix(const Matrix4& matWS);
		const Matrix4&					GetWorldMatrix() const;

		SubMaterial*					GetMaterial() const {return m_pSubMaterial.get();}

		virtual uint32_t				GetRenderOrder() { return m_nRenderOrder; }

	public:
		PRIMITIVE_TYPE					m_ePrimitiveType;
		RefPtr<VertexBuffer>			m_pVertexBuffer;	
		RefPtr<IndexBuffer>				m_pIndexBuffer;
		RefPtr<SubMeshData>				m_pSubMeshData;
		RefPtr<SubMaterial>				m_pSubMaterial;

		bool							m_bSuportInstace = false;
		
		uint32_t						m_nRenderOrder = 0;

	private:
		Matrix4							m_matWorld[2];
	};

}

