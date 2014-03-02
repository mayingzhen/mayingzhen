#ifndef  _FbxImportSkeletonMesh__H__
#define  _FbxImportSkeletonMesh__H__


namespace ma
{
	struct pointSkin
	{
		std::vector<std::string>	m_vBoneName;
		std::vector<UINT>			m_vBoneInd;
		std::vector<float>			m_vBoneWeight;
	};
	
	
	FbxSkeleton* GetFbxRootBone(FbxNode* pNode);

	void GetSkeletonData(FbxSkeleton* pSkeleton,FbxPose* pBindPose,Skeleton& skeData);
 
	void GetSkinInfo(const FbxMesh* pMesh,const Skeleton& skeData, std::vector<pointSkin>& vPointSkin, int& clusterCount);
	
	void SkinDataCovert(UINT& skinBoneID,UINT& skinBoneWeight,const std::vector<UINT>& arrBone,const std::vector<float>& arrWeight);
	
	template<class VertexType,class IndexType>
	void GetSkinMeshData(FbxMesh* pMesh,MeshData* pMeshData,const Skeleton& skeData,ImportParm* pImportParm);
	
	template<class VertexType>
	void GetSkinData(FbxMesh* pMesh,int nTriangleIndex, VertexType vertex[3],std::vector<pointSkin>& arrPointSkin, ImportParm* pImportParm);
}

#endif



