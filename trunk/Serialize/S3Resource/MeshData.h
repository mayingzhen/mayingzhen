#ifndef __S3MeshData_H__
#define __S3MeshData_H__

//#include "Common/RTTI/Object.h"

namespace S3
{

	enum MeshVersion
	{
		MESH_VER_INITIAL = 1,
		MESH_VER_USE_SERIALZIE,
		MESH_VER_EMBED_MESH_DATA,
		MESH_VER_ANIMATION_RETARGET,
		MESH_VER_LOD_MESH_DATA,
		MESH_VER_CURRENT = MESH_VER_LOD_MESH_DATA
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
		VT_SKIN_VERTEX_0 = 0,
	};

	struct ENGINE_API VertexType0 
	{
	public:
		D3DXVECTOR3    p;				// position
		xmUint32	 b;				// bones
		xmUint32	 w;				// Weights		
		D3DXVECTOR3	 n;				// normal
		D3DXVECTOR2	 uv;				// Texture Coordinate
		D3DXVECTOR3	 tgn;
		xmUint32     vc;				// vertex color
		xmUint32	 vi;				// vertex illumination

	// 	 bool operator != ( const VertexType0& other) const
	// 	 {
	// 		 if ( p != other.p || uv != other.uv)
	// 			 return false;
	// 		 else
	// 			 return true;
	// 	 }

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
		D3DXVECTOR3 m_vMin;
		D3DXVECTOR3 m_vMax;

		void Init();

		void Merge(const D3DXVECTOR3& vMin,const D3DXVECTOR3& vMax);

		void AddPoint(const D3DXVECTOR3& v);
	};

	struct ENGINE_API Bounding 
	{
	public:
		xmUint16			m_nShapeType;
		D3DXVECTOR3		m_vPos;
		D3DXQUATERNION	m_qRot;

		//union
		//{
			BoxShape		m_boxShape;
			CylinderShape m_cylinderShape;
		//};

		void SetInvalid();

		void SetIdentity();

		void SetAABB(const D3DXVECTOR3& vMin,const D3DXVECTOR3& vMax);

		void GetAABB(D3DXVECTOR3& vMin,D3DXVECTOR3& vMax) const;

		void SetOBB(const D3DXVECTOR3* pPos,const D3DXQUATERNION* pRot
			,float fXSize,float fYSize,float fZSize);

		void Serialize(SerializeListener& sl, const char* pszLable = "Bounding" );
	};


	struct ENGINE_API MeshHeader 
	{
	public:
		xmUint32		m_nIdent;	 // File Identity = 'S3MD'
		xmUint32		m_nVersion;	 // 格式版本号
		std::string m_strMaxFile;
		GUID		m_nSkelGUID;
		UINT		m_nBoneNum;
		UINT		m_nIndexNum;
		UINT		m_nVertexNum;

		void Serialize(SerializeListener& sl,const char* pszLable = "MeshHeader");
	};



	struct ENGINE_API SubMeshData /*: public ISubMeshData*/
	{
	public:
		UINT m_nIndexStart;
		UINT m_nIndexCount;
		UINT m_nVertexStart;
		UINT m_nVertexCount;

		UINT m_nMateiralID;

		Bounding				m_subMeshBound;
		std::vector<BoneIndex>	m_arrBonePalette;
		std::string					m_name;
		std::string					m_submeshTag;
		

	public:

		SubMeshData();

		void	Reset(UINT nIndexStart,UINT nIndexCount
			,UINT nVertexStart,UINT nVertexCount
			,UINT nMtlID
			,const Bounding* pBounding
			,const BoneIndex* arrBonePalatte
			,UINT nBonePalatteSize = 0		
			,const char* pszName = NULL
			,const char* pszTag = NULL);

		const char* GetName() const;

		const char*	GetTag() const;

		UINT	GetIndexStart() const;

		UINT	GetIndexCount() const;

		UINT	GetVertexStart() const;

		UINT	GetVertexCount() const;

		UINT	GetMaterialID() const;

		Bounding          &GetMeshBounding() { return m_subMeshBound; }

		BoneIndex			GetBonePaletteSize() const;

		const BoneIndex*		GetBonePalette() const;

		//------------------------------------------------------------------------------
		//For Modification
		//------------------------------------------------------------------------------
		 void	SetName(const char* pszName);

		 void	SetTag(const char* pszTag);


		 void	SetIndexStart(xmUint32 nIndStart);

		 void	SetIndexCount(xmUint32 nIndCnt);

		 void	SetVertexStart(xmUint32 nVStart);

		 void	SetVertexCount(xmUint32 nVCnt);

		 void	SetMaterialID(UINT nMtlID);

		/*!\brief
		\param arrBoneIndice array of bone indices, if null, invalid bone index will be filled
		*/
		 void	ResetBonePalatte(xmUint32 nPalatteSize,xmUint16* arrBoneIndice);

		 void	SetBoneByPalatteIndex(xmUint16 nPalatteInd,xmUint16 nBoneInd);

		 xmUint16	GetBoneByPalatteIndex(xmUint16 nPalatteInd);

		void	Serialize(SerializeListener& sl,const char* pszLabel = "SubMeshData");

	};


	struct ENGINE_API MeshLODData /*: public IMeshLODData*/
	{
	public:
		std::vector<SubMeshData*> m_arrSubMesh;

		enum MeshDataFrom
		{
			DATA_PARENT_DATA = 0,
			DATA_HAS_OWN_INDEX = 1,
			DATA_HAS_OWN_VERTEX = 2,
			DATA_HAS_BONE_LOD = 4
		};

	public:
		UINT m_nMeshDataFlag;  // MeshDataFrom 位组合

		// DATA_HAS_OWN_INDEX
		UINT m_nIndexType;
		std::vector<xmUint8>	m_arrIndexBuffer;
		
		// -------------------
		// ===================

		// DATA_HAS_OWN_VERTEX
		UINT m_nVertexType;
		std::vector<xmUint8>	m_arrVertexBuffer;

		// -------------------
		// ===================
		// 对应骨骼级别
		int m_nBoneLOD;

	public:

		MeshLODData();
		~MeshLODData();

		UINT					GetSubMeshNumber() const;

		const SubMeshData*		GetSubMesh(UINT nSubMeshInd) const;

		void					ClearSubMesh();

		void					ResizeSubMesh(UINT nSubMeshNum);

		SubMeshData*			CreateSubMesh();

		SubMeshData*			GetSubMesh(UINT nSubMeshInd);

		int						GetBoneLOD() const;

		void					SetBoneLOD(int nBoneLOD);

		UINT					GetMeshDataFlag() const;

		void					SetMeshDataFlag(UINT uMeshDataFlag);

		UINT					GetIndexNumber() const;

		bool					GetIsIndex32() const;

		void*					GetIndexBuffer() { return &m_arrIndexBuffer[0]; }

		VertexType0*			GetVertexBufferVertexType0();

		int						GetVertexNumber() const;

		void					ResetBuffer(bool bIsIndex32, xmUint32 nIndexNum, xmUint32 nVertexNum);

		void					Serialize(SerializeListener& sl,const char* pszLabel = "MeshLodData");
	};

	struct ENGINE_API MeshData /* : public IMeshData*/
	{
	public:
		MeshHeader			m_header;

		UINT m_nIndexType;
		UINT m_nVertexType;

		std::vector<xmUint8>			m_arrIndexBuffer;
		std::vector<xmUint8>			m_arrVertexBuffer;

		Bounding					m_meshBound;
		std::vector<MeshLODData*>	m_arrMeshLOD;

		//std::vector<std::string>		m_arrBoneName;
		//std::vector<Bounding>		m_arrBoneBound;
		

	private:

		void					Clear();

		void RemoveDegenerateTriangleInplace(std::vector<xmUint8> &arrIndexBuffer, UINT nIndexType, std::vector<unsigned int> &subIndexMove);

		void DegenerateSubMeshData( MeshLODData * pLODData, std::vector<unsigned int> &subIndexMove );

	public:

		MeshHeader &GetMeshHeader(void)
		{
			return m_header;
		}

		MeshData();

		~MeshData();


		void				Release();

		//------------------------------------------------------------------------------
		//File Header
		//------------------------------------------------------------------------------
		const char*			GetSource() const;

		GUID				GetGlobalSkeletonID() const;

		//------------------------------------------------------------------------------
		//Stream Source
		//------------------------------------------------------------------------------
		UINT				GetIndexNumber() const;

		bool				GetIsIndex32() const;

		const void*			GetIndexBuffer() const;

		UINT				GetVertexNumber() const;

		UINT				GetVertexSize() const;

		UINT				GetVertexStride() const;

		UINT				GetBoneIndexOffset() const;

		UINT				GetBoneWeightOffset() const;


		VertexType0*	GetVertexBufferVertexType0();

		const VertexType0*	GetVertexBufferVertexType0() const;

		void RemoveDegenerateTriangleInplace(void);


		//------------------------------------------------------------------------------
		//Skin info
		//------------------------------------------------------------------------------
		//UINT				GetBoneNumber() const;

		//const char*			GetBoneName(UINT nBoneInd) const;


		void				GetBoundingAABB(D3DXVECTOR3* vMin,D3DXVECTOR3* vMax);

		//------------------------------------------------------------------------------
		//For LOD Mesh
		//------------------------------------------------------------------------------
		UINT					GetLODMeshNumber() const;
		MeshLODData*			GetLODMesh(int nLOD);
		const MeshLODData*		GetLODMesh(int nLOD) const;
		int						GetBoneLOD(int nMeshLOD) const;
		void					SetBoneLOD(int nMeshLOD, int nBoneLOD);
		MeshLODData*			IncLOD();
		void					DecLOD();
		void					SetLodMeshVersion();

		//------------------------------------------------------------------------------
		//For modification
		//------------------------------------------------------------------------------

		void					ResetBuffer(
			UINT nIndexType,xmUint32 nIndexNum
			,xmUint32 nVertexNum
			,xmUint32 nSubMeshNum
			,xmUint32 nBoneNum
			);

		void					SetSource(const char* pszSource);

		void					SetGlobalSkeletonID(GUID nSkelGUID);


		void*					GetIndexBuffer();

		VertexType0*		GetVertexBuffer();	


		//------------------------------------------------------------------------------
		//Sub mesh
		//------------------------------------------------------------------------------
		UINT					GetSubMeshNumber() const;

		const SubMeshData*		GetSubMesh(UINT nSubMeshInd, int nLOD = 0) const;

		SubMeshData*			GetSubMesh(UINT nSubMeshInd, int nLOD = 0);

		SubMeshData*			GetSubMeshByName(const char* pszName, int nLOD = 0);


		//void					ResetBone(xmUint32 nBoneNum);

		//void					SetBoneName(BoneIndex nBoneInd,const char* pszBoneName);

		//void					SetBoneBoundOBB(BoneIndex nBoneInd,const D3DXVECTOR3* pPos,const D3DXQUATERNION* pRot,float fXSize,float fYSize,float fZSize);

		void					SetBoundingAABB(const D3DXVECTOR3* vMin,const D3DXVECTOR3* vMax);

		void					Serialize(SerializeListener& sl,const char* pszLabel = "MeshData");

	};
}

#endif// __MeshData_H__ 