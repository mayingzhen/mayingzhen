#include "Common/RenderSystem/Mesh.h"

namespace ma
{

	void SubMesh::Serialize(SerializeListener& sl,const char* pszLabel)
	{

	}

	SubMesh* Mesh::CreateSubMesh()
	{
		SubMesh* pSubMesh = new SubMesh();
		mSubMeshs.push_back(pSubMesh);
		return pSubMesh;
	}

	void Mesh::Serialize(SerializeListener& sl,const char* pszLabel)
	{

	}
}

