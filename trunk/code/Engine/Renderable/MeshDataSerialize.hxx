

namespace ma
{
	enum MeshVersion
	{
		MESH_VER_INITIAL = 1,

		MESH_VER_CURRENT = MESH_VER_INITIAL,
	};


	struct RENDER_API MeshHeader 
	{

		uint32		m_nIdent;	 // File Identity 
		uint32		m_nVersion;	 // ∏Ò Ω∞Ê±æ∫≈

		MeshHeader()
		{
			m_nIdent = 'MAMD';
			m_nVersion = MESH_VER_CURRENT;
		}
	};


	void SerializeData(Serializer& sl, AABB& val,const char* pszLable)
	{
		sl.BeginSection(pszLable);
		
		//ASSERT(false);

		if ( sl.IsReading() )
		{
			Vector3 vMin;
			Vector3 vMax;

			sl.Serialize(vMin,"m_vMin");
			sl.Serialize(vMax,"m_vMax");

			val.setMinimum(vMin);
			val.setMaximum(vMax);
		}
		else
		{
			Vector3 vMin = val.getMinimum();
			Vector3 vMax = val.getMaximum();

			sl.Serialize(vMin,"m_vMin");
			sl.Serialize(vMax,"m_vMax");
		}

		sl.EndSection();
	}


	void SerializeData(Serializer& sl, MeshHeader& mehHeader, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(mehHeader.m_nIdent,"FileIden");
		sl.Serialize(mehHeader.m_nVersion,"Version");	

		sl.EndSection();
	}

	void SerializeData(Serializer& sl, SubMeshData& subMeshData, const char* pszLabel)
	{
		sl.BeginSection(pszLabel);

		sl.Serialize(subMeshData.m_nIndexStart,"IndexStart");
		sl.Serialize(subMeshData.m_nIndexCount,"IndexCount");
		sl.Serialize(subMeshData.m_nVertexStart,"VertexStart");
		sl.Serialize(subMeshData.m_nVertexCount,"VertexCount");
		sl.Serialize(subMeshData.m_arrBonePalette,"BonePalette");
		sl.Serialize(subMeshData.m_nMateiralID,"MaterialID");

		sl.EndSection();
	}

	void MeshData::SerializeDataV0(Serializer& sl, const char* pszLabel)
	{
		sl.BeginSection(pszLabel);

		sl.Serialize(m_nIndexType,"IndexType");
		sl.Serialize(m_nVertexType,"VertexType");
		sl.Serialize(*m_pVertexBuffer,"VertexBuffer");
		sl.Serialize(*m_pIndexBuffer,"IndexBuffer");
		sl.Serialize(m_meshBound,"MeshBound");
		sl.Serialize(m_arrSubMesh,"SubMesh");

		sl.EndSection();
	}

}

