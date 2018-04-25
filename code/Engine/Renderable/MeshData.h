#ifndef __MeshData_H__
#define __MeshData_H__

#include "Engine/RenderSystem/IRenderDevice/IndexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexDeclaration.h"

namespace ma
{
	struct SHORTV4
	{
		int16_t x;
		int16_t y;
		int16_t z;
		int16_t w;
	};

	struct SHORTV2
	{
		int16_t x;
		int16_t y;
	};

	uint32_t CompressNormal(const Vector3& vNorm);
	uint32_t CompressQuaternion(const Vector3& tangent,Vector3 const& normal); 
	SHORTV4 CompressPos(const Vector3& vPos, const Vector3& vCenter, const Vector3& vExtent);
	SHORTV2 CompressUV(const Vector2& vUV,const Vector2& vCenter, const Vector2& vExtent);

	Vector3 DecompressPos(SHORTV4 vPos, const Vector3& vCenter, const Vector3& vExtent);
	

	struct SkinVertexV0
	{
		Vector3 pos;
		uint32_t  bone_index;
		uint32_t  bone_weight;
		Vector3 nor;
		Vector2 uv;
		Vector3 tan;
		uint32_t  color0;
		uint32_t  color1;

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
		SHORTV4 pos;
		uint32_t	nor;
		SHORTV2	uv;
		uint32_t  bone_index;
		uint32_t  bone_weight;
	}; //24B

	struct StaticVertexV1
	{
		SHORTV4 pos;
		uint32_t	nor;
		SHORTV2	uv;
	}; //16B

	enum VertexType
	{
		SKIN_VERTEX_0 = 0,
		SKIN_VERTEX_1 = 1,
		STATIC_VERTEX_1 = 2,
	};	

	class SubMeshData : public Referenced
	{
	public:
		uint32_t					m_nIndexStart;
		uint32_t					m_nIndexCount;
		uint32_t					m_nVertexStart;
		uint32_t					m_nVertexCount;
		uint32_t					m_nMateiralID;
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
		
		uint32_t					GetSubMeshNumber() {return (uint32_t)m_arrSubMesh.size();}
		SubMeshData*			GetSubMeshByIndex(uint32_t index) const {return m_arrSubMesh[index].get();} 
		void					AddSubMeshData(SubMeshData* pSubMeshData);

		IndexBuffer*			GetIndexBuffer() const {return m_pIndexBuffer.get();} 

		VertexBuffer*			GetVertexBuffer() const {return m_pVertexBuffer.get();} 

		VertexDeclaration*		GetVertexDeclar() const {return m_pDeclaration.get();}
	
		void					SetBoundingAABB(const AABB& bound) {m_meshBound = bound;}
		const AABB&				GetBoundingAABB() const {return m_meshBound;} 

		void					SetUVBoundingAABB(const AABB2D& bound) {m_tcBound = bound;}
		const AABB2D&			GetUVBoundingAABB() const {return m_tcBound;} 

		VertexType				GetVertexType() const {return m_nVertexType;}
		void					SetVertexType(VertexType nType) {m_nVertexType = nType;}

		INDEX_TYPE				GetIndexType() const {return m_nIndexType;}
		void					SetIndexType(INDEX_TYPE nType) {m_nIndexType = nType;} 

		uint32_t				GetBoneNumber() const {return m_nBoneNumber;}
		void					SetBoneNumber(uint32_t nNumber) {m_nBoneNumber = nNumber;}
		
		bool					SaveToFile(const char* pszFile);	

		void					UpdateMeshData(SkinVertexV1* pVertexV1,SkinVertexV0* pVertexV0,uint32_t nVertexCount,uint16_t* pIndex);
		void					UpdateMeshData(StaticVertexV1* pVertexV1,SkinVertexV0* pVertexV0,uint32_t nVertexCount,uint16_t* pIndex);	
	
	private:

		bool					InitRes();
		
		void					SaveData();
		void					ReadDataV1();
		void					ReadDataV0();

	private:
		uint32_t							m_nBoneNumber;
		INDEX_TYPE						m_nIndexType;
		VertexType						m_nVertexType;
		RefPtr<IndexBuffer>				m_pIndexBuffer;
		RefPtr<VertexBuffer>			m_pVertexBuffer;
		RefPtr<VertexDeclaration>		m_pDeclaration; 
		
		typedef std::vector< RefPtr<SubMeshData> > VEC_SUBMESH;
		VEC_SUBMESH						m_arrSubMesh;	

		AABB							m_meshBound;
		AABB2D							m_tcBound;

	};
	
	RefPtr<MeshData> CreateMeshData(const char* pszFile);
	RefPtr<MeshData> CreateMeshData();

	extern ResourceSystem<MeshData>* g_pMeshManager;
}

#endif// __MeshData_H__ 
