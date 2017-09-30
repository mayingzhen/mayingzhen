#ifndef  _Rendererable__H__
#define  _Rendererable__H__


namespace ma
{
	class SubMaterial;
	class Technique;
	class MeshData;
	class SubMeshData;

	class Renderable : public Referenced
	{
	public:
		Renderable();

		virtual void					PreRender(Technique* pTech);

		//virtual void					Render(Technique* pTech);

		void							SetWorldMatrix(const Matrix4& matWS);
		const Matrix4&					GetWorldMatrix() const;

		SubMaterial*					GetMaterial() const {return m_pSubMaterial.get();}

	public:
		PRIMITIVE_TYPE					m_ePrimitiveType;
		RefPtr<VertexBuffer>			m_pVertexBuffer;	
		RefPtr<IndexBuffer>				m_pIndexBuffer;
		RefPtr<SubMeshData>				m_pSubMeshData;
		RefPtr<SubMaterial>				m_pSubMaterial;

		void*							m_pCommand = NULL;
	
	private:
		Matrix4							m_matWorld[2];
	};

}

#endif

