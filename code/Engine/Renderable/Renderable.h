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

		SubMaterial*					GetMaterial() const {return m_pSubMaterial.get();}

	public:
		PRIMITIVE_TYPE					m_ePrimitiveType;
		RefPtr<VertexDeclaration>		m_pDeclaration;
		RefPtr<VertexBuffer>			m_pVertexBuffers;	
		RefPtr<IndexBuffer>				m_pIndexBuffer;
		RefPtr<SubMeshData>				m_pSubMeshData;
		RefPtr<SubMaterial>				m_pSubMaterial;

		AABB							m_posAABB;
		AABB2D							m_tcAABB;
	
	private:
		Matrix4							m_matWorld[2];
	};

	class SkinRenderable : public Renderable
	{
	public:
		virtual void					Render(Technique* pTech);

		void							SetSkinMatrix(const Matrix3x4* arrMatrixs, uint32 nCount);

		const DualQuaternion*			GetSkinDQ() const;
		const float*					GetSkinScale() const;
		uint32							GetSkinDQCount() const;
		const Matrix3x4*				GetSkinMatrix() const;

	private:
		typedef std::vector<DualQuaternion>	VEC_DQ;
		typedef std::vector<float> VEC_SCALE;
		typedef std::vector<Matrix3x4> VEC_MATRIX;
		VEC_DQ							m_arrSkinDQ[2];
		VEC_SCALE						m_arrScale[2];
		VEC_MATRIX						m_arrSkinMatrix[2];
	};

}

#endif

