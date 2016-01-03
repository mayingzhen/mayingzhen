#ifndef __MeshData_H__
#define __MeshData_H__

#include "Engine/RenderSystem/IRenderDevice/IndexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexDeclaration.h"

namespace ma
{

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

	struct StaticVertexV1
	{
		Vector3 pos;
		uint32	nor;
		Vector2	uv;
		uint32  color;
	}; //28B

	enum VertexType
	{
		SKIN_VERTEX_0 = 0,
		SKIN_VERTEX_1 = 1,
		STATIC_VERTEX_1 = 2,
	};	

	enum IndexType
	{
		INDEX_16 = 0,
		INDEX_32 = 1,
	};


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

		VertexType				GetVertexType() const {return m_nVertexType;}
		void					SetVertexType(VertexType nType) {m_nVertexType = nType;}

		IndexType				GetIndexType() const {return m_nIndexType;}
		void					SetIndexType(IndexType nType) {m_nIndexType = nType;} 
		
		bool					SaveToFile(const char* pszFile);	

		void					UpdateMeshData(SkinVertexV1* pVertexV1,SkinVertexV0* pVertexV0,uint32 nVertexCount,UINT16* pIndex);
		void					UpdateMeshData(StaticVertexV1* pVertexV1,SkinVertexV0* pVertexV0,uint32 nVertexCount,UINT16* pIndex);

	private:

		bool					InitRes();
		
		void					SaveData();
		void					ReadDataV1();
		void					ReadDataV0();

	private:
		
		IndexType						m_nIndexType;
		VertexType						m_nVertexType;
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