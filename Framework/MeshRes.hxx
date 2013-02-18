#include "Framework/MeshRes.h"

namespace ma
{
	MeshRes::MeshRes(const char* pszPath):
	Resource(pszPath)
	{
		m_pRendMesh = NULL;
	}

	MeshRes::~MeshRes()
	{

	}

	bool MeshRes::Load()
	{
		if (m_pRendMesh == NULL)
		{
			m_pRendMesh = GetRenderDevice()->CreateRendMesh();
		}
		
		MeshData* pMeshData = LoadMeshFromBinaryFile(m_sResPath.c_str());
		
		m_pRendMesh->InitWithData(pMeshData);
		return true;
	}

	bool MeshRes::IsLoad()
	{
		//m_pRendMesh->
		return true;
	}

	void MeshRes::Serialize(SerializeListener& sl, const char* pszLable)
	{
		__super::Serialize(sl,pszLable);
	}
}
