#ifndef __MeshData_H__
#define __MeshData_H__

#include "Engine/RenderSystem/IRenderDevice/IndexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexDeclaration.h"

namespace ma
{

	struct  V_3P_2UV_3N_3T
	{
		Vector3  m_position;			// position
		Vector2	 m_uv;					// Texture Coordinate
		Vector3	 m_normal;				// normal
		//Vector3	 m_tangent;				// normal

		bool operator == ( const V_3P_2UV_3N_3T& other) const
		{
			if (m_position == other.m_position && m_uv == other.m_uv)
				return true;
			else
				return false;
		}
	};

	struct  V_3P_2UV_3N_3T_S
	{
		Vector3  m_position;			// position
		Vector2	 m_uv;					// Texture Coordinate
		Vector3	 m_normal;				// normal
		//Vector3	 m_tangent;				// normal
		uint32	 m_boneID;				// bones
		uint32	 m_weight;				// Weights	

		bool operator == ( const V_3P_2UV_3N_3T_S& other) const
		{
			if (m_position == other.m_position && m_uv == other.m_uv)
				return true;
			else
				return false;
		}
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

	DeclareRefPtr(SubMeshData);


	class IndexBuffer;
	class VertexBuffer;
	class VertexDeclaration;

	class RENDER_API MeshData : public Resource
	{
		DECL_OBJECT(MeshData)

	public:
		MeshData(const char* pszPath = NULL);

		~MeshData();
		
		UINT					GetSubMeshNumber() {return m_arrSubMesh.size();}

		SubMeshDataPtr			GetSubMeshByIndex(UINT index) const {return m_arrSubMesh[index];} 

		IndexBufferPtr			GetIndexBuffer() const {return m_pIndexBuffer;} 

		VertexBufferPtr			GetVertexBuffer() const {return m_pVertexBuffer;} 

		VertexDeclarationPtr	GetVertexDeclar() const {return m_pDeclaration;}
	
		void					SetBoundingAABB(const AABB& bound) {m_meshBound = bound;}
		const AABB&				GetBoundingAABB() const {return m_meshBound;} 

		UINT					GetVertexType() const {return m_nVertexType;}
		void					SetVertexType(UINT nType) {m_nVertexType = nType;}

		UINT					GetIndexType() const {return m_nIndexType;}
		void					SetIndexType(UINT nType) {m_nIndexType = nType;} 

		SubMeshData*			AddSubMeshData();

		virtual void			Serialize(Serializer& sl, const char* pszLable = "MeshData");

	private:
		int						GetVertexStride();

		void					SerializeDataV0(Serializer& sl, const char* pszLabel);

	private:
		
		UINT							m_nIndexType;
		UINT							m_nVertexType;
		IndexBufferPtr					m_pIndexBuffer;
		VertexBufferPtr					m_pVertexBuffer;
		VertexDeclarationPtr			m_pDeclaration; 

		std::vector<SubMeshDataPtr>		m_arrSubMesh;	

		AABB							m_meshBound;		
	};

	DeclareRefPtr(MeshData);
	
}

#endif// __MeshData_H__ 