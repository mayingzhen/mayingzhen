#ifndef __MeshData_H__
#define __MeshData_H__


namespace ma
{

	struct  V_3P_2UV_3N_3T
	{
		Vector3  m_position;			// position
		Vector2	 m_uv;					// Texture Coordinate
		Vector3	 m_normal;				// normal
		//Vector3	 m_tangent;				// normal

		bool operator == ( const V_3P_2UV_3N_3T& other) const
		{
			if (m_position == other.m_position && m_uv == other.m_uv)
				return true;
			else
				return false;
		}
	};

	struct  V_3P_2UV_3N_3T_S
	{
		Vector3  m_position;			// position
		Vector2	 m_uv;					// Texture Coordinate
		Vector3	 m_normal;				// normal
		//Vector3	 m_tangent;				// normal
		Uint32	 m_boneID;				// bones
		Uint32	 m_weight;				// Weights	

		bool operator == ( const V_3P_2UV_3N_3T_S& other) const
		{
			if (m_position == other.m_position && m_uv == other.m_uv)
				return true;
			else
				return false;
		}
	};


	struct SubMeshData 
	{

		UINT					m_nIndexStart;
		UINT					m_nIndexCount;
		UINT					m_nVertexStart;
		UINT					m_nVertexCount;
		UINT					m_nMateiralID;
		std::vector<BoneIndex>	m_arrBonePalette;

		SubMeshData()
		{
			m_nIndexStart = 0;
			m_nIndexCount = 0;
			m_nVertexStart = 0;
			m_nVertexCount = 0;
			m_nMateiralID = 0;
		}
	};


	class IndexBuffer;
	class VertexBuffer;
	class VertexDeclaration;

	class RENDER_API MeshData : public Resource
	{
		DECL_OBJECT(MeshData)

	public:
		MeshData();
		
		UINT					GetSubMeshNumber() {return m_arrSubMesh.size();}

		SubMeshData*			GetSubMeshByIndex(UINT index) {return m_arrSubMesh[index];}

		IndexBuffer*			GetIndexBuffer() {return m_pIndexBuffer;}

		VertexBuffer*			GetVertexBuffer() {return m_pVertexBuffer;}

		VertexDeclaration*		GetVertexDeclar() {return m_pDeclaration;}

		AABB					GetBoundingAABB() {return m_meshBound;}

		virtual void			Serialize(Serializer& sl, const char* pszLable = "MeshData");

	private:
		bool					InitRendable();

		int						GetVertexStride();

	public:
		
		UINT						m_nIndexType;
		UINT						m_nVertexType;
		IndexBuffer*				m_pIndexBuffer;
		VertexBuffer*				m_pVertexBuffer;
		VertexDeclaration*			m_pDeclaration; 

		std::vector<SubMeshData*>	m_arrSubMesh;	

		AABB						m_meshBound;		
	};

	DeclareRefPtr(MeshData);
	
}

#endif// __MeshData_H__ 