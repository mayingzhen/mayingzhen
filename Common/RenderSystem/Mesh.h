#ifndef __Mesh_H__
#define __Mesh_H__

#include "Common/Resource.h"
#include "Common/RenderSystem/PrimtiveStream.h"

namespace ma
{
// 	struct COMMON_API AABBShape 
// 	{
// 	public:
// 		D3DXVECTOR3 m_vMin;
// 		D3DXVECTOR3 m_vMax;
// 
// 		void Init();
// 
// 		void Merge(const D3DXVECTOR3& vMin,const D3DXVECTOR3& vMax);
// 
// 		void AddPoint(const D3DXVECTOR3& v);
// 	};

	struct COMMON_API SubMesh 
	{
	public:
		UINT					GetBonePaletteSize() const {return m_arrBonePalette.size();}

		const BoneIndex*		GetBonePalette() const {return &m_arrBonePalette[0];}

		void					Serialize(SerializeListener& sl,const char* pszLabel = "SubMeshData");

	protected:
		VertexStream            mVertexStream;
		IndexStream             mIndexStream;
		int                     mPrimCount;
		PRIMITIVE_TYPE          mPrimType;
		Material				mMaterial;	

		std::vector<BoneIndex>	m_arrBonePalette;

	};

	struct COMMON_API Mesh : public Resource 
	{
	public:
		xmUint					GetSubMeshNumber() const {return mSubMeshs.size();}

		SubMesh*				GetSubMeshByIndex(xmUint uIndex) {return mSubMeshs[uIndex];}

		SubMesh *				CreateSubMesh();

		void					Serialize(SerializeListener& sl,const char* pszLabel = "Mesh");

	protected:
		std::vector<SubMesh*>	mSubMeshs;
		
		//AABBShape				m_meshBound;
	};
}

#endif// __MeshData_H__ 
