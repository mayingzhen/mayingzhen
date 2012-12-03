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
		m_pRendMesh->Load( m_sResPath.c_str() );
	}

	bool MeshRes::IsLoad()
	{
		//m_pRendMesh->
		return true;
	}
}

