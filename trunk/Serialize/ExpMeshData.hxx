#include "Serialize/ExpMeshData.h"
#include "Serialize/Serialize/SerializeListener.h"

void ExpVertexType0::Serialize(SerializeListener& sl,const char* pszLabel)
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

void ExpBoxShape::Serialize(SerializeListener& sl,const char* pszLabel )
{
	sl.BeginSection(pszLabel);
	sl.Serialize(m_fXSize,"XSize");
	sl.Serialize(m_fYSize,"YSize");
	sl.Serialize(m_fZSize,"ZSize");
	sl.EndSection();
}

void ExpAABBShape::Init()
{
	m_vMin = xmVector3(xm_FMAX,xm_FMAX,xm_FMAX);
	m_vMax = xmVector3(-xm_FMAX,-xm_FMAX,-xm_FMAX);
}

void ExpAABBShape::Merge(const xmVector3& vMin,const xmVector3& vMax)
{
	xmVec3Min(&m_vMin,&vMin,&m_vMin);
	xmVec3Max(&m_vMax,&vMax,&m_vMax);
}

void ExpAABBShape::AddPoint(const xmVector3& v)
{
	xmVec3Min(&m_vMin,&v,&m_vMin);
	xmVec3Max(&m_vMax,&v,&m_vMax);
}

void ExpCylinderShape::Serialize(SerializeListener& sl, const char* pszLable )
{
	sl.BeginSection(pszLable);
	sl.Serialize(m_fRadius,"Radius");
	sl.Serialize(m_fHeight,"Height");
	sl.EndSection();
}

void ExpBounding::Serialize(SerializeListener& sl, const char* pszLable)
{
	sl.BeginSection(pszLable);
	sl.Serialize(m_nShapeType,"ShapeType");
	sl.Serialize(m_vPos,"Position");
	sl.Serialize(m_qRot,"Rotation");
	if (EXP_BS_BOX == m_nShapeType)
	{
		sl.Serialize(m_boxShape,"BoxShape");
	}
	else if (EXP_BS_CYLINDER == m_nShapeType)
	{
		sl.Serialize(m_cylinderShape,"CylinderShape");
	}
	else{
		SSERT(false && "unknown bounding shape type");
	}

	sl.EndSection();
}


void ExpSubMesh::Serialize(SerializeListener& sl, const char* pszLable)
{
	sl.BeginSection(pszLable);

	sl.Serialize(m_nIndexStart,"IndexStart");
	sl.Serialize(m_nIndexCount,"IndexCount");
	sl.Serialize(m_nVertexStart,"VertexStart");
	sl.Serialize(m_nVertexCount,"VertexCount");
	sl.Serialize(m_subMeshBound,"SubMeshBound");
	sl.Serialize(m_name,"SubMeshName");
	sl.Serialize(m_submeshTag,"SubMeshTag");
	sl.Serialize(m_arrBonePalette,"BonePalette");


	sl.EndSection();
}


void ExpMeshLOD::Serialize(SerializeListener& sl, const char* pszLable)
{
	sl.BeginSection(pszLable);
	sl.Serialize(m_arrSubMesh,"SubMesh");
	sl.EndSection();
}

void ExpMeshData::Serialize(SerializeListener& sl,const char* pszLable)
{
	sl.BeginSection(pszLable);

	sl.Serialize(m_nIndexType,"IndexType");
	sl.Serialize(m_nVertexType,"VertexType");

	if (EXP_INDEX_TYPE_U16 == m_nIndexType)
	{
		sl.SerializeRawData<BoneIndex>(m_arrIndexBuffer,"IndexBuffer");
	}
	else if (EXP_INDEX_TYPE_U32 == m_nIndexType)
	{
		sl.SerializeRawData<xmUint32>(m_arrIndexBuffer,"IndexBuffer");
	}

	if (EXP_VT_SKIN_VERTEX_0 == m_nVertexType)
	{
		sl.SerializeRawData<ExpVertexType0>(m_arrVertexBuffer,"VertexBuffer");
	}

	sl.Serialize(m_arrBoneName,"BoneName");

	sl.Serialize(m_arrMeshLOD,"MeshLOD");

	sl.Serialize(m_meshBound,"MeshBound");

	sl.EndSection();
}


void ExpMeshHeader::Serialize(SerializeListener& sl,const char* pszLable)
{
	sl.BeginSection(pszLable);

	sl.Serialize(m_nIdent,"FileIden");
	sl.Serialize(m_nVersion,"Version");
	sl.Serialize(m_strMaxFile,"SourceFile");
	if (m_nVersion <= EXP_MESH_VER_EMBED_MESH_DATA)
	{
		sl.Serialize(m_nSkelGUID.m_a,"SkeletonGUID");
		m_nSkelGUID.m_b = 0;
	}else{
		sl.Serialize(m_nSkelGUID,"SkeletonGUID");
	}

	sl.Serialize(m_nBoneNum,"BoneNumber");
	sl.Serialize(m_nIndexNum,"IndexNumber");
	sl.Serialize(m_nVertexNum,"VertexNumber");

	sl.EndSection();
}


void ExpMesh::Serialize(SerializeListener& sl, const char* pszLable)
{
	sl.BeginSection(pszLable);

	sl.Serialize(m_header,"MeshHeader");

	sl.PushVersion(m_header.m_nVersion);
	sl.Serialize(m_mesh,"MeshData");
	sl.PopVersion();

	sl.EndSection();
}
