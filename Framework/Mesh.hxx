#include "Mesh.h"

namespace ma
{
	Mesh::Mesh(const char* pszPath):
	Resource(pszPath)
	{
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Load()
	{
		if (m_pRendMesh == NULL)
		{
			m_pRendMesh = GetRender()->CreateRendMesh();
		}
		m_pRendMesh->Load( m_sResPath.c_str() );
	}

	bool Mesh::IsLoad()
	{
		//m_pRendMesh->
		return true;
	}
}

