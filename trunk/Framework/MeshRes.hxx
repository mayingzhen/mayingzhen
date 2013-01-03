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

	void MeshRes::Load()
	{
		if (m_pRendMesh == NULL)
		{
			m_pRendMesh = GetRenderDevice()->CreateRendMesh();
		}
		
		MeshData* pMeshData = LoadMeshFromBinaryFile(m_sResPath.c_str());
		
		m_pRendMesh->InitWithData(pMeshData);
	}

	bool MeshRes::IsLoad()
	{
		//m_pRendMesh->
		return true;
	}
}

