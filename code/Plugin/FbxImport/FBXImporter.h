#ifndef __FBXImporter_H__
#define __FBXImporter_H__

namespace fbxsdk_2013_2
{
	class FbxManager;
	class FbxImporter;
	class FbxMesh;
	class FbxSkeleton;
	class FbxSurfaceMaterial;
	class FbxAnimStack;
	class FbxNode;
	class FbxPose;
	class FbxScene;
}

using namespace fbxsdk_2013_2;

namespace ma
{
	class MeshData;

	class FBXImporter_API FBXImporter
	{

	public:
		FBXImporter();

		~FBXImporter();

		bool	Initialize();
	
		bool	LoadStaticMeshData(const char* pFileName,MeshData* pMeshData);

		bool	LoadSkeletonMeshData(const char* pFileName,MeshData* pMeshData,SkeletonData* pSkeData);

		bool	LoadAnimationData(const char* pFileName,AnimationData* pAnimation,const SkeletonData* pSkelData);

	private:
		FbxScene*	GetFbxScene(const char* pFileName);

		FbxMesh* GetFbxMesh(FbxNode* pNode);

		FbxSkeleton* GetFbxRootBone(FbxNode* pNode);

		void GetMeshData(FbxMesh* pMesh,MeshData* pMeshData,const SkeletonData* pSkelData);

		void GetSkeletonData(FbxSkeleton* pSkeleton,FbxPose* pBindPose,SkeletonData* pSkelData);

		void GetAnimtionData(FbxNode* pNode,std::vector<AnimationData*> m_vAnimData);

		void ReadVertex(FbxMesh* pMesh , int ctrlPointIndex , Vector3* pVertex);

		void ReadColor(FbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , Vector4* pColor);

		void ReadUV(FbxMesh* pMesh , int ctrlPointIndex , int textureUVIndex , int uvLayer , Vector2* pUV);

		void ReadNormal(FbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , Vector3* pNormal);

		void ReadTangent(FbxMesh* pMesh , int ctrlPointIndex , int vertecCounter , Vector3* pTangent);

		// Material
		void ConnectMaterialToMesh(FbxMesh* pMesh , int triangleCount , int* pTriangleMtlIndex);

		void LoadMaterial(FbxMesh* pMesh);

		void LoadMaterialAttribute(FbxSurfaceMaterial* pSurfaceMaterial);

		void LoadMaterialTexture(FbxSurfaceMaterial* pSurfaceMaterial);
	
	private:
		FbxManager*		m_pFBXSDKManager;
		FbxImporter*	m_pFBXImporter;
		//FbxScene*		m_pFBXSDKScene;
	};
}


#endif
