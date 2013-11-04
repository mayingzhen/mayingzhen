

namespace ma
{
	enum MeshVersion
	{
		MESH_VER_INITIAL = 1,

		MESH_VER_CURRENT = MESH_VER_INITIAL,
	};


	struct RENDER_API MeshHeader 
	{

		Uint32		m_nIdent;	 // File Identity 
		Uint32		m_nVersion;	 // ∏Ò Ω∞Ê±æ∫≈

		MeshHeader()
		{
			m_nIdent = 'MAMD';
			m_nVersion = MESH_VER_CURRENT;
		}
	};


	void SerializeData(Serializer& sl, AABB& val,const char* pszLable)
	{
		sl.BeginSection(pszLable);
		
		sl.Serialize(val.m_vMin,"m_vMin");
		sl.Serialize(val.m_vMax,"m_vMax");

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

	void SerializeDataV0(Serializer& sl, MeshData& meshData, const char* pszLabel)
	{
		sl.BeginSection(pszLabel);

		sl.Serialize(meshData.m_nIndexType);
		sl.Serialize(meshData.m_nVertexType);
		sl.Serialize(meshData.m_arrIndexBuffer);
		sl.Serialize(meshData.m_arrVertexBuffer);	
		sl.Serialize(meshData.m_meshBound,"MeshBound");
		sl.Serialize(meshData.m_arrSubMesh,"SubMesh");
		sl.Serialize(meshData.m_nSkelGUID,"SkeletonGUID");

		sl.EndSection();
	}

}

