#include "MeshData.h"
#include "Common/RTTI/Object.h"


namespace ma
{
	void VertexType0::Serialize(SerializeListener& sl,const char* pszLabel)
	{
		sl.BeginSection(pszLabel);

		sl.Serialize(p,"Position");
		sl.Serialize(b,"BoneIndice");
		sl.Serialize(w,"BoneWeight");
		sl.Serialize(n,"Normal");
		sl.Serialize(uv,"UV");
		sl.Serialize(tgn,"Tangent");
		sl.Serialize(vc,"VertexColor");
		sl.Serialize(vi,"VertexIllumniation");

		sl.EndSection();

	}


	void BoxShape::Serialize(SerializeListener& sl,const char* pszLabel )
	{
		sl.BeginSection(pszLabel);
		sl.Serialize(m_fXSize,"XSize");
		sl.Serialize(m_fYSize,"YSize");
		sl.Serialize(m_fZSize,"ZSize");
		sl.EndSection();
	}


	void AABBShape::Init()
	{
		m_vMin = Vector3(FMAX,FMAX,FMAX);
		m_vMax = Vector3(-FMAX,-FMAX,-FMAX);
	}

	void AABBShape::Merge(const Vector3& vMin,const Vector3& vMax)
	{
		Vec3Min(&m_vMin,&vMin,&m_vMin);
		Vec3Max(&m_vMax,&vMax,&m_vMax);
	}

	void AABBShape::AddPoint(const Vector3& v)
	{
		Vec3Min(&m_vMin,&v,&m_vMin);
		Vec3Max(&m_vMax,&v,&m_vMax);
	}


	void CylinderShape::Serialize(SerializeListener& sl, const char* pszLable )
	{
		sl.BeginSection(pszLable);
		sl.Serialize(m_fRadius,"Radius");
		sl.Serialize(m_fHeight,"Height");
		sl.EndSection();
	}


	void Bounding::Serialize(SerializeListener& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);
		sl.Serialize(m_nShapeType,"ShapeType");
		sl.Serialize(m_vPos,"Position");
		sl.Serialize(m_qRot,"Rotation");
		if (BS_BOX == m_nShapeType)
		{
			sl.Serialize(m_boxShape,"BoxShape");
			//m_boxShape.Serialize(sl);
		}
		else if (BS_CYLINDER == m_nShapeType)
		{
			sl.Serialize(m_cylinderShape,"CylinderShape");
		}
		else
		{
			//assert(false && "unknown bounding shape type");
		}

		sl.EndSection();
	}

	void MeshHeader::Serialize(SerializeListener& sl,const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_nIdent,"FileIden");
		sl.Serialize(m_nVersion,"Version");
		sl.Serialize(m_nSkelGUID,"SkeletonGUID");
// 		sl.Serialize(m_nBoneNum,"BoneNumber");
// 		sl.Serialize(m_nIndexNum,"IndexNumber");
// 		sl.Serialize(m_nVertexNum,"VertexNumber");

		sl.EndSection();
	}


	/*!
	param val
	Null element is not allowed in array.
	T must be a simple struct or class which do not has derived class.
	*/
	template<class T>
	void Serialize(SerializeListener& sl,std::vector<T*>& val,const char* pszLable = "array")
	{
		sl.BeginSection(pszLable);

		UINT nSize = (UINT)val.size();
		sl.Serialize(nSize,"size");

		if (nSize != val.size())
		{
			val.resize(nSize);
		}
		sl.BeginSection("element");

		for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
		{
			char buf[32];
			sprintf_s(&buf[0],32,"Element_%u",nCnt);
			
			//Null element is not allowed in array
			if (val[nCnt] == NULL)
			{
				val[nCnt] = new T;//T must be a simple struct or class which do not has derived class
			}

			sl.Serialize(*val[nCnt],buf);
		}
		sl.EndSection();

		sl.EndSection();
	}

	template<class DataType>
	void SerializeRawData(SerializeListener& sl,std::vector<Uint8>& val,const char* pszLable)
	{
		sl.BeginSection(pszLable);

		UINT nSize = val.size();
		sl.Serialize(nSize,"size");

		if (nSize != val.size())
		{
			val.resize(nSize);
		}

		sl.BeginSection("element");

		UINT nDataNum = nSize / sizeof(DataType);

		for (UINT nCnt = 0;nCnt < nDataNum; ++nCnt)
		{
			char buf[64];
			sprintf_s(&buf[0],64,"Element_%u",nCnt);
			sl.Serialize((DataType&)val[nCnt*sizeof(DataType)],buf);
		}
		sl.EndSection();

		sl.EndSection();
	}

	SubMeshData::SubMeshData()
	:m_nIndexStart(0)
	,m_nIndexCount(0)
	,m_nVertexStart(0)
	,m_nVertexCount(0)
	,m_nMateiralID(0)
	{
		
	}


	UINT	SubMeshData::GetIndexStart() const
	{
		return m_nIndexStart;
	}


	UINT	SubMeshData::GetIndexCount() const
	{
		return m_nIndexCount;
	}


	UINT	SubMeshData::GetVertexStart() const
	{
		return m_nVertexStart;
	}


	UINT	SubMeshData::GetVertexCount() const
	{
		return m_nVertexCount;
	}

	UINT	SubMeshData::GetMaterialID() const
	{
		return m_nMateiralID;
	}

	BoneIndex	SubMeshData::GetBonePaletteSize() const
	{
		return m_arrBonePalette.size();
	}

	const BoneIndex*	SubMeshData::GetBonePalette() const
	{
		return m_arrBonePalette.size() > 0 ? &m_arrBonePalette[0] : NULL;
	}


	void	SubMeshData::SetIndexStart(Uint32 nIndStart)
	{
		m_nIndexStart = nIndStart;
	}


	void	SubMeshData::SetIndexCount(Uint32 nIndCnt)
	{
		m_nIndexCount = nIndCnt;
	}


	void	SubMeshData::SetVertexStart(Uint32 nVStart)
	{
		m_nVertexStart = nVStart;
	}


	void	SubMeshData::SetVertexCount(Uint32 nVCnt)
	{
		m_nVertexCount = nVCnt;
	}

	void	SubMeshData::SetMaterialID(UINT nMtlID)
	{
		m_nMateiralID = nMtlID;
	}

	void	SubMeshData::SetBoneByPalatteIndex(Uint16 nPalatteInd,Uint16 nBoneInd)
	{
		m_arrBonePalette[nPalatteInd] = nBoneInd;
	}


	Uint16	SubMeshData::GetBoneByPalatteIndex(Uint16 nPalatteInd)
	{
		return m_arrBonePalette[nPalatteInd];
	}


	void	SubMeshData::Serialize(SerializeListener& sl,const char* pszLabel)
	{
		sl.BeginSection(pszLabel);

		sl.Serialize(m_nIndexStart,"IndexStart");
		sl.Serialize(m_nIndexCount,"IndexCount");
		sl.Serialize(m_nVertexStart,"VertexStart");
		sl.Serialize(m_nVertexCount,"VertexCount");
		sl.Serialize(m_arrBonePalette,"BonePalette");
		sl.Serialize(m_nMateiralID,"MaterialID");

		sl.EndSection();
	}



	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	MeshData::MeshData()
	:m_nIndexType(INDEX_TYPE_U16)
	,m_nVertexType(VT_SKIN_VERTEX_0)
	{
	}

	MeshData::~MeshData()
	{
	}

	maGUID MeshData::GetGlobalSkeletonID() const
	{
		return m_header.m_nSkelGUID;
	}


	UINT MeshData::GetIndexNumber() const
	{
		return m_arrIndexBuffer.size() / (GetIsIndex32() ? sizeof(Uint32) : sizeof(Uint16));
	}


	bool MeshData::GetIsIndex32() const
	{
		return m_nIndexType == INDEX_TYPE_U32;
	}

	const void*			MeshData::GetIndexBuffer() const
	{
		return m_arrIndexBuffer.size() > 0 ? &m_arrIndexBuffer[0]: 0;
	}

	UINT	MeshData::GetVertexSize() const
	{
		UINT nSize = sizeof(Uint8);
		if (m_nVertexType == VT_SKIN_VERTEX_0)
		{
			nSize = sizeof(VertexType0);
		}
		return nSize;
	}

	UINT						MeshData::GetVertexNumber() const
	{
		return m_arrVertexBuffer.size() / GetVertexSize();
	}

	VertexType0*	MeshData::GetVertexBufferVertexType0()
	{
		Uint8* pData = m_arrVertexBuffer.size() > 0
			? &m_arrVertexBuffer[0]
		: NULL;

		return VT_SKIN_VERTEX_0 == m_nVertexType 
			? reinterpret_cast<VertexType0*>(pData)
			: NULL;
	}

	const VertexType0*	MeshData::GetVertexBufferVertexType0() const
	{
		const Uint8* pData = m_arrVertexBuffer.size() > 0
			? &m_arrVertexBuffer[0]
			: NULL;

		return VT_SKIN_VERTEX_0 == m_nVertexType 
			? reinterpret_cast<const VertexType0*>(pData)
			: NULL;
	}

	void MeshData::GetBoundingAABB(Vector3* vMin,Vector3* vMax)
	{
		m_meshBound.GetAABB(*vMin,*vMax);
	}

	void* MeshData::GetIndexBuffer()
	{
		return m_arrIndexBuffer.size() > 0 ? &m_arrIndexBuffer[0] : 0;
	}


	VertexType0*		MeshData::GetVertexBuffer()
	{
		return (VertexType0*)(m_arrVertexBuffer.size() > 0 ? &m_arrVertexBuffer[0] : 0);
	}
		

	void MeshData::Serialize(SerializeListener& sl,const char* pszLabel)
	{
		sl.BeginSection(pszLabel);

		sl.Serialize(m_header,"MeshHeader");

		sl.PushVersion(m_header.m_nVersion);
		sl.Serialize(m_nIndexType,"IndexType");
		sl.Serialize(m_nVertexType,"VertexType");

		if (INDEX_TYPE_U16 == m_nIndexType)
		{
			sl.SerializeRawData<BoneIndex>(m_arrIndexBuffer,"IndexBuffer");
		}
		else if (INDEX_TYPE_U32 == m_nIndexType)
		{
			sl.SerializeRawData<Uint32>(m_arrIndexBuffer,"IndexBuffer");
		}
		else
		{
			Log("Fail to serialize mesh data : invalid index type");
		}

		if (VT_SKIN_VERTEX_0 == m_nVertexType)
		{
			sl.SerializeRawData<VertexType0>(m_arrVertexBuffer,"VertexBuffer");
		}

		sl.Serialize(m_meshBound,"MeshBound");
 		sl.Serialize(m_arrSubMesh,"SubMesh");

		sl.PopVersion();

		sl.EndSection();
	}

	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------


	void Bounding::SetInvalid()
	{
		m_nShapeType = BS_UNKNOWN;
	}

	void Bounding::SetIdentity()
	{
		Vector3 vMin(-0.5f,-0.5f,-0.5f);
		Vector3 vMax(0.5f,0.5f,0.5f);
		SetAABB(vMin,vMax);
	}

	void Bounding::SetAABB(const Vector3& vMin,const Vector3& vMax)
	{
		m_nShapeType = BS_BOX;
		m_vPos = 0.5f*(vMin + vMax);
		QuaternionIdentity(&m_qRot);
		m_boxShape.m_fXSize = vMax.x - vMin.x;
		m_boxShape.m_fYSize = vMax.y - vMin.y;
		m_boxShape.m_fZSize = vMax.z - vMin.z;
	}

	void Bounding::SetOBB(const Vector3* pPos,const Quaternion* pRot
				,float fXSize,float fYSize,float fZSize)
	{
		m_nShapeType = BS_BOX;
		//m_vPos = pPos ? *pPos : Vec3Zero();
		//m_qRot = pRot ? *pRot : QuaternionIden();
		m_boxShape.m_fXSize = fXSize;
		m_boxShape.m_fYSize = fYSize;
		m_boxShape.m_fZSize = fZSize;

	}

	void Bounding::GetAABB(Vector3& vMin,Vector3& vMax) const
	{

		if (m_nShapeType == BS_BOX)
		{
			Vector3 vSize(m_boxShape.m_fXSize,m_boxShape.m_fYSize,m_boxShape.m_fZSize);
			vSize = vSize * 0.5f;
			vMin = m_vPos - vSize;
			vMax = m_vPos + vSize;
		}else{
			//LogError(0,"Fail to get AABB");
			vMin = m_vPos;
			vMax = m_vPos;
		}
	}

	void Serialize(SerializeListener& sl,VertexType0 &val,const char* pszLable )
	{
		sl.BeginSection(pszLable);

		sl.Serialize(val.p,"Position");
		sl.Serialize(val.b,"BoneIndice");
		sl.Serialize(val.w,"BoneWeight");
		sl.Serialize(val.n,"Normal");
		sl.Serialize(val.uv,"UV");
		sl.Serialize(val.tgn,"Tangent");
		sl.Serialize(val.vc,"VertexColor");
		sl.Serialize(val.vi,"VertexIllumniation");

		sl.EndSection();

	}

	void Serialize(SerializeListener& sl,BoxShape &val,const char* pszLable )
	{
		sl.BeginSection(pszLable);
		sl.Serialize(val.m_fXSize,"XSize");
		sl.Serialize(val.m_fYSize,"YSize");
		sl.Serialize(val.m_fZSize,"ZSize");
		sl.EndSection();
	}

	void Serialize(SerializeListener& sl,CylinderShape &val,const char* pszLable )
	{
		sl.BeginSection(pszLable);
		sl.Serialize(val.m_fRadius,"Radius");
		sl.Serialize(val.m_fHeight,"Height");
		sl.EndSection();
	}

}

