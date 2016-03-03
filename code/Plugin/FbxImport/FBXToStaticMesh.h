#ifndef  _FbxImportStaticMesh__H__
#define  _FbxImportStaticMesh__H__

namespace ma
{
	struct subMaterial
	{
		int				m_matId;
		std::string		m_strDiff;
		std::string		m_strEmiss;
		std::string		m_strNormal;
	};

	void GetMeshData(FbxMesh* pMesh,MeshData* pMeshData,ImportParm* pImportParm);

	void UpdateVertexArray(std::vector<SkinVertexV0>& arrVertex,std::vector<UINT16>& arrIndex,SkinVertexV0 vertex);

	Material*  CreateMeshMaterial(FbxMesh* pMesh,int materiID,ImportParm* pImportParm,bool bSkin);
	
	void GetTriangleData(FbxMesh* pMesh,int nTriangleIndex, SkinVertexV0 vertex[3],ImportParm* pImportParm);

	void GetTextureNames( FbxProperty &pProperty, std::string& pConnectionString );

	void GetMaterialTextureData( FbxSurfaceMaterial* pMaterial, subMaterial& smData);

	void GetSubMaterialData(FbxMesh* pMesh, std::vector<subMaterial>& arrSubmaterial);

	void ConnectMaterialToMesh(FbxMesh* pMesh, std::map< int,std::vector<int> >& matIndexToTriangleIndex);

	void ReadVertex(FbxMesh* pMesh , int ctrlPointIndex , Vector3* pVertex);

	void ReadColor(FbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , Vector4* pColor);

	void ReadUV(FbxMesh* pMesh , int ctrlPointIndex , int textureUVIndex , int uvLayer , Vector2* pUV);

	void ReadNormal(FbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , Vector3* pNormal);

	void ReadTangent(FbxMesh* pMesh , int ctrlPointIndex , int vertecCounter , Vector3* pTangent);

	void LoadMaterialTexture(FbxMesh* pMesh,int materialID,std::string& strDiff,std::string strNormal);

	void LoadMaterialTexture(FbxSurfaceMaterial* pSurfaceMaterial);

}

#endif



