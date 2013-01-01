#ifndef __FBXImporter_H__
#define __FBXImporter_H__

// class KFbxNode;
// class KFbxMesh;
// class FbxManager;

namespace ma
{
	class SERIALIZE_API FBXImporter
	{

	public:
		FBXImporter();

		~FBXImporter();

		bool Initialize();
	
		MeshData* LoadScene(const char* pSeneName);

	private:
		void ProcessNode(MeshData* pMeshData, FbxNode* pNode);

		void ProcessMesh(MeshData* pMeshData, FbxNode* pNode);

		void ReadVertex(FbxMesh* pMesh , int ctrlPointIndex , D3DXVECTOR3* pVertex);

		void ReadColor(FbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , D3DXVECTOR4* pColor);

		void ReadUV(FbxMesh* pMesh , int ctrlPointIndex , int textureUVIndex , int uvLayer , D3DXVECTOR2* pUV);

		void ReadNormal(FbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , D3DXVECTOR3* pNormal);

		void ReadTangent(FbxMesh* pMesh , int ctrlPointIndex , int vertecCounter , D3DXVECTOR3* pTangent);

		// Material
		void ConnectMaterialToMesh(FbxMesh* pMesh , int triangleCount , int* pTriangleMtlIndex);

		void LoadMaterial(FbxMesh* pMesh);

		void LoadMaterialAttribute(FbxSurfaceMaterial* pSurfaceMaterial);

		void LoadMaterialTexture(FbxSurfaceMaterial* pSurfaceMaterial);
	
	private:
		FbxManager* mpFBXSDKManager;
		//FbxScene*   mpFBXSDKScene;
	};
}


#endif