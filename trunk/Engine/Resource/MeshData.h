#ifndef __MeshData_H__
#define __MeshData_H__


namespace ma
{

	struct ENGINE_API V_3P_2UV_3N_3T
	{
		Vector3  m_position;			// position
		Vector2	 m_uv;					// Texture Coordinate
		//Vector3	 m_normal;				// normal
		//Vector3	 m_tangent;				// normal

		bool operator == ( const V_3P_2UV_3N_3T& other) const
		{
			if (m_position == other.m_position && m_uv == other.m_uv)
				return true;
			else
				return false;
		}
	};

	struct ENGINE_API V_3P_2UV_3N_3T_S
	{
		Vector3  m_position;			// position
		Vector2	 m_uv;					// Texture Coordinate
		//Vector3	 m_normal;				// normal
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


	struct ENGINE_API MeshHeader 
	{

		Uint32		m_nIdent;	 // File Identity 
		Uint32		m_nVersion;	 // ∏Ò Ω∞Ê±æ∫≈
		
		MeshHeader();
	};



	struct ENGINE_API SubMeshData 
	{

		UINT					m_nIndexStart;
		UINT					m_nIndexCount;
		UINT					m_nVertexStart;
		UINT					m_nVertexCount;
		UINT					m_nMateiralID;
		std::vector<BoneIndex>	m_arrBonePalette;

		SubMeshData::SubMeshData()
		{
			m_nIndexStart = 0;
			m_nIndexCount = 0;
			m_nVertexStart = 0;
			m_nVertexCount = 0;
			m_nMateiralID = 0;
		}
	};



	class ENGINE_API MeshData : public Resource
	{

	public:
		MeshData();

		UINT			GetSubMeshNumber() {return m_arrSubMesh.size();}
		SubMeshData*	GetSubMeshByIndex(UINT index) {return m_arrSubMesh[index];}

		void*			GetIndexBuffer() {return &m_arrIndexBuffer[0];}
		UINT			GetIndexBufferSize() {return m_arrIndexBuffer.size();}
		UINT		GetIndexType() {return m_nIndexType;}
		
		void*			GetVertexBuffer() {return &m_arrVertexBuffer[0];}
		UINT			GetVertexBufferSize() {return m_arrVertexBuffer.size();}	
		UINT			GetVertexType() {return m_nVertexType;}

		virtual void	Serialize(SerializeListener& sl, const char* pszLable = "MeshData");


	public:
		
		UINT						m_nIndexType;
		Uint						m_nVertexType;
		std::vector<Uint8>			m_arrIndexBuffer;
		std::vector<Uint8>			m_arrVertexBuffer;

		AABB						m_meshBound;		
		std::vector<SubMeshData*>	m_arrSubMesh;
		maGUID						m_nSkelGUID;
	
	};
	
}

#endif// __MeshData_H__ 