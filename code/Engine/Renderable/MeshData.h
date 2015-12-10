#ifndef __MeshData_H__
#define __MeshData_H__

#include "Engine/RenderSystem/IRenderDevice/IndexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexDeclaration.h"

namespace ma
{

// 	struct  V_3P_2UV_3N_3T
// 	{
// 		Vector3  m_position;			// position
// 		Vector2	 m_uv;					// Texture Coordinate
// 		Vector3	 m_normal;				// normal
// 		//Vector3	 m_tangent;				// normal
// 
// 		bool operator == ( const V_3P_2UV_3N_3T& other) const
// 		{
// 			if (m_position == other.m_position && m_uv == other.m_uv)
// 				return true;
// 			else
// 				return false;
// 		}
// 	};

	struct SkinVertexV0
	{
		Vector3 pos;
		uint32  bone_index;
		uint32  bone_weight;
		Vector3 nor;
		Vector2 uv;
		Vector3 tan;
		uint32  color0;
		uint32  color1;

		bool operator == ( const SkinVertexV0& other) const
		{
			if (pos == other.pos && uv == other.uv)
				return true;
			else
				return false;
		}
	}; // 60B

	struct SkinVertexV1
	{
		Vector3 pos;
		uint32	nor;
		Vector2	uv;
		uint32  bone_index;
		uint32  bone_weight;
		uint32  color;
	}; //36B


	class SubMeshData : public Referenced
	{
	public:
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

		void AddBonePalette(BoneIndex bonde);
	};

	RefPtr<SubMeshData>  CreateSubMeshData();

	class IndexBuffer;
	class VertexBuffer;
	class VertexDeclaration;

	class MeshData : public Resource
	{
	
	public:
		MeshData();

		~MeshData();
		
		uint32					GetLodNumerber() {return m_arrLodSubMesh.size();}
		UINT					GetSubMeshNumber(uint32 nLod) {return m_arrLodSubMesh[nLod].size();}
		SubMeshData*			GetSubMeshByIndex(uint32 nLod,UINT index) const {return m_arrLodSubMesh[nLod][index].get();} 
		void					AddSubMeshData(uint32 nLod,SubMeshData* pSubMeshData);

		IndexBuffer*			GetIndexBuffer() const {return m_pIndexBuffer.get();} 

		VertexBuffer*			GetVertexBuffer() const {return m_pVertexBuffer.get();} 

		VertexDeclaration*		GetVertexDeclar() const {return m_pDeclaration.get();}
	
		void					SetBoundingAABB(const AABB& bound) {m_meshBound = bound;}
		const AABB&				GetBoundingAABB() const {return m_meshBound;} 

		UINT					GetVertexType() const {return m_nVertexType;}
		void					SetVertexType(UINT nType) {m_nVertexType = nType;}

		UINT					GetIndexType() const {return m_nIndexType;}
		void					SetIndexType(UINT nType) {m_nIndexType = nType;} 
		
		bool					SaveToFile(const char* pszFile);	

	private:

		bool					InitRes();
		
		void					SaveData();
		void					ReadDataV1();
		void					ReadDataV0();

	private:
		
		UINT							m_nIndexType;
		UINT							m_nVertexType;
		RefPtr<IndexBuffer>				m_pIndexBuffer;
		RefPtr<VertexBuffer>			m_pVertexBuffer;
		RefPtr<VertexDeclaration>		m_pDeclaration; 

		typedef std::vector< RefPtr<SubMeshData> > VEC_SUBMESH;
		typedef std::vector< VEC_SUBMESH > VEC_LOD_SUBMESH;
		VEC_LOD_SUBMESH					m_arrLodSubMesh;	

		AABB							m_meshBound;		
	};
	
	RefPtr<MeshData> CreateMeshData(const char* pszFile);
	RefPtr<MeshData> CreateMeshData();

	extern ResourceSystem<MeshData>* g_pMeshManager;
}

#endif// __MeshData_H__ 