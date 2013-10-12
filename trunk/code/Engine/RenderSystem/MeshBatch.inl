#include "MeshBatch.h"


namespace ma
{

// 	Material* MeshBatch::getMaterial() const
// 	{
// 		return m_pMaterial;
// 	}

	template <class T>
	void MeshBatch::add(const T* vertices, unsigned int vertexCount, const unsigned short* indices, unsigned int indexCount)
	{
		ASSERT(sizeof(T) == m_pDeclaration->GetStreanmStride());
		add(vertices, sizeof(T), vertexCount, indices, indexCount);
	}

}
