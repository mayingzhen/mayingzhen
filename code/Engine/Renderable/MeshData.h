#ifndef __MeshData_H__
#define __MeshData_H__

#include "Engine/RenderSystem/IRenderDevice/IndexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexDeclaration.h"

namespace ma
{
	struct SHORTV4
	{
		int16 x;
		int16 y;
		int16 z;
		int16 w;
	};

	struct SHORTV2
	{
		int16 x;
		int16 y;
	};

	UINT32 CompressNormal(const Vector3& vNorm);
	UINT32 CompressQuaternion(const Vector3& tangent,Vector3 const& normal); 
	SHORTV4 CompressPos(const Vector3& vPos, const Vector3& vCenter, const Vector3& vExtent);
	SHORTV2 CompressUV(const Vector2& vUV,const Vector2& vCenter, const Vector2& vExtent);
	

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
		SHORTV4 pos;
		uint32	nor;
		SHORTV2	uv;
		uint32  bone_index;
		uint32  bone_weight;
	}; //24B

	struct StaticVertexV1
	{
		SHORTV4 pos;
		uint32	nor;
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
		
		UINT					GetSubMeshNumber() {return m_arrSubMesh.size();}
		SubMeshData*			GetSubMeshByIndex(UINT index) const {return m_arrSubMesh[index].get();} 
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

		UINT32					GetBoneNumber() const {return m_nBoneNumber;}
		void					SetBoneNumber(UINT32 nNumber) {m_nBoneNumber = nNumber;}
		
		bool					SaveToFile(const char* pszFile);	

		void					UpdateMeshData(SkinVertexV1* pVertexV1,SkinVertexV0* pVertexV0,uint32 nVertexCount,uint16* pIndex);
		void					UpdateMeshData(StaticVertexV1* pVertexV1,SkinVertexV0* pVertexV0,uint32 nVertexCount,uint16* pIndex);	
	
	private:

		bool					InitRes();
		
		void					SaveData();
		void					ReadDataV1();
		void					ReadDataV0();

	private:
		UINT							m_nBoneNumber;
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
