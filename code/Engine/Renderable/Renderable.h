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

		virtual void					Render(Technique* pTech);

		void							SetWorldMatrix(const Matrix4& matWS);
		const Matrix4&					GetWorldMatrix() const;

		void							SetSkinMatrix(const Matrix3x4* arrMatrixs, uint32 nCount);
		const DualQuaternion*			GetSkinDQ() const;
		uint32							GetSkinDQCount() const;

		SubMaterial*					GetMaterial() const {return m_pMaterial.get();}

	public:
		PRIMITIVE_TYPE					m_ePrimitiveType;
		RefPtr<VertexDeclaration>		m_pDeclaration;
		RefPtr<VertexBuffer>			m_pVertexBuffers;	
		RefPtr<IndexBuffer>				m_pIndexBuffer;
		RefPtr<SubMeshData>				m_pSubMeshData;
		RefPtr<SubMaterial>				m_pMaterial;
	
	private:
		Matrix4							m_matWorld[2];
		typedef std::vector<DualQuaternion>	VEC_DQ;
		VEC_DQ							m_arrSkinDQ[2];
	};

	DeclareRefPtr(Renderable);

}

#endif
