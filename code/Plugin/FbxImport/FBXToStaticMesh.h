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

	template<class VertexType,class IndexType>
	void GetMeshData(FbxMesh* pMesh,MeshData* pMeshData,ImportParm* pImportParm);

	template<class VertexType,class IndexType>
	void UpdateVertexArray(std::vector<VertexType>& arrVertex,std::vector<IndexType>& arrIndex,VertexType vertex);

	MaterialData*  CreateDefaultMaterial(FbxMesh* pMesh,int materiID,ImportParm* pImportParm,bool bSkin);
	
	template<class VertexType>
	void GetTriangleData(FbxMesh* pMesh,int nTriangleIndex, VertexType vertex[3],ImportParm* pImportParm);

	template<class VertexType,class IndexType>
	void UpdateHardwareBuffer(std::vector<VertexType>& arrVertex,std::vector<IndexType>& arrIndex,
		VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer);

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



