#ifndef __MeshData_H__
#define __MeshData_H__

//#include "Common/RTTI/Object.h"

namespace ma
{

	enum MeshVersion
	{
		MESH_VER_INITIAL = 1,

		MESH_VER_CURRENT = MESH_VER_INITIAL,
	};

	enum IndexType
	{
		INDEX_TYPE_U16 = 0,
		INDEX_TYPE_U32
	};

	enum BoundingShapeType
	{
		BS_BOX = 0,
		BS_CYLINDER,
		BS_NUM,
		BS_UNKNOWN = 0xffff
	};

	enum VertexType
	{
		VT_SKIN_VERTEX_0,
		VT_3f_3f_2f,
	};

	struct ENGINE_API VertexType0 
	{
	public:
		Vector3    p;				// position
		Uint32	 b;				// bones
		Uint32	 w;				// Weights		
		Vector3	 n;				// normal
		Vector2	 uv;				// Texture Coordinate
		Vector3	 tgn;
		Uint32     vc;				// vertex color
		Uint32	 vi;				// vertex illumination

		 bool operator == ( const VertexType0& other) const
		 {
			 if (p == other.p && uv == other.uv)
				 return true;
			 else
				 return false;
		 }

		void	Serialize(SerializeListener& sl,const char* pszLabel = "Vertex");
	};

	struct ENGINE_API VT_3F_3F_2F 
	{
	public:
		Vector3  p;				// position
		Vector3	 n;				// normal
		Vector2	 uv;			// Texture Coordinate

		bool operator == ( const VertexType0& other) const
		{
			if (p == other.p && uv == other.uv)
				return true;
			else
				return false;
		}

		void	Serialize(SerializeListener& sl,const char* pszLabel = "Vertex");
	};


	struct ENGINE_API CylinderShape
	{
	public:
		float m_fRadius;
		float m_fHeight;

		void Serialize(SerializeListener& sl, const char* pszLable = "CylinderShape" );

	};

	struct ENGINE_API BoxShape 
	{
	public:
		float m_fXSize;
		float m_fYSize;
		float m_fZSize;

		void	Serialize(SerializeListener& sl,const char* pszLabel = "BoxShape");

	};

	struct ENGINE_API AABBShape 
	{
	public:
		Vector3 m_vMin;
		Vector3 m_vMax;

		void Init();

		void Merge(const Vector3& vMin,const Vector3& vMax);

		void AddPoint(const Vector3& v);
	};

	struct ENGINE_API Bounding 
	{
	public:
		Uint16			m_nShapeType;
		Vector3		m_vPos;
		Quaternion	m_qRot;

		//union
		//{
			BoxShape		m_boxShape;
			CylinderShape m_cylinderShape;
		//};

		void SetInvalid();

		void SetIdentity();

		void SetAABB(const Vector3& vMin,const Vector3& vMax);

		void GetAABB(Vector3& vMin,Vector3& vMax) const;

		void SetOBB(const Vector3* pPos,const Quaternion* pRot
			,float fXSize,float fYSize,float fZSize);

		void Serialize(SerializeListener& sl, const char* pszLable = "Bounding" );
	};


	struct ENGINE_API MeshHeader 
	{
	public:
		Uint32	m_nIdent;	 // File Identity 
		Uint32	m_nVersion;	 // ∏Ò Ω∞Ê±æ∫≈
		maGUID		m_nSkelGUID;
// 		UINT		m_nBoneNum;
// 		UINT		m_nIndexNum;
// 		UINT		m_nVertexNum;

		MeshHeader()
		{
			m_nIdent = 'S3MD';
			m_nVersion = MESH_VER_CURRENT;
			m_nSkelGUID.Clear();
// 			m_nBoneNum = 0;
// 			m_nIndexNum	= 0;
// 			m_nVertexNum = 0;
		}

		void Serialize(SerializeListener& sl,const char* pszLable = "MeshHeader");
	};



	struct ENGINE_API SubMeshData 
	{
	public:
		UINT m_nIndexStart;
		UINT m_nIndexCount;
		UINT m_nVertexStart;
		UINT m_nVertexCount;

		UINT m_nMateiralID;

		//Bounding				m_subMeshBound;
		std::vector<BoneIndex>	m_arrBonePalette;
		//std::string				m_name;
		//std::string				m_submeshTag;
		

	public:
		SubMeshData();

		const char* GetName() const;

		const char*	GetTag() const;

		UINT	GetIndexStart() const;

		UINT	GetIndexCount() const;

		UINT	GetVertexStart() const;

		UINT	GetVertexCount() const;

		UINT	GetMaterialID() const;

		//Bounding          &GetMeshBounding() { return m_subMeshBound; }

		BoneIndex			GetBonePaletteSize() const;

		const BoneIndex*		GetBonePalette() const;

		//------------------------------------------------------------------------------
		//For Modification
		//------------------------------------------------------------------------------
		 void	SetName(const char* pszName);

		 void	SetTag(const char* pszTag);


		 void	SetIndexStart(Uint32 nIndStart);

		 void	SetIndexCount(Uint32 nIndCnt);

		 void	SetVertexStart(Uint32 nVStart);

		 void	SetVertexCount(Uint32 nVCnt);

		 void	SetMaterialID(UINT nMtlID);

		/*!\brief
		\param arrBoneIndice array of bone indices, if null, invalid bone index will be filled
		*/
		 void	ResetBonePalatte(Uint32 nPalatteSize,Uint16* arrBoneIndice);

		 void	SetBoneByPalatteIndex(Uint16 nPalatteInd,Uint16 nBoneInd);

		 Uint16	GetBoneByPalatteIndex(Uint16 nPalatteInd);

		void	Serialize(SerializeListener& sl,const char* pszLabel = "SubMeshData");

	};



	struct ENGINE_API MeshData : public Resource
	{
	public:
		MeshHeader					m_header;
		UINT						m_nIndexType;
		UINT						m_nVertexType;
		std::vector<Uint8>			m_arrIndexBuffer;
		std::vector<Uint8>			m_arrVertexBuffer;
		Bounding					m_meshBound;	
		std::vector<SubMeshData*>	m_arrSubMesh;
		
	public:
		MeshData();

		~MeshData();

		maGUID				GetGlobalSkeletonID() const;

		UINT				GetIndexNumber() const;

		bool				GetIsIndex32() const;

		const void*			GetIndexBuffer() const;

		UINT				GetVertexSize() const;

		UINT				GetVertexNumber() const;

		VertexType0*		GetVertexBufferVertexType0();

		const VertexType0*	GetVertexBufferVertexType0() const;

		void				GetBoundingAABB(Vector3* vMin,Vector3* vMax);

		void*				GetIndexBuffer();

		VertexType0*		GetVertexBuffer();	


		//------------------------------------------------------------------------------
		//Sub mesh
		//------------------------------------------------------------------------------
		UINT					GetSubMeshNumber() const {return m_arrSubMesh.size();}

		const SubMeshData*		GetSubMesh(UINT nSubMeshInd) const {return m_arrSubMesh[nSubMeshInd];}

		SubMeshData*			GetSubMesh(UINT nSubMeshInd) {return m_arrSubMesh[nSubMeshInd];}

		void					Serialize(SerializeListener& sl,const char* pszLabel = "MeshData");

	};
	
}

#endif// __MeshData_H__ 