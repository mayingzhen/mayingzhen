#ifndef _MeshBatch_H_
#define _MeshBatch_H_

#include "Renderable.h"

namespace ma
{
	class MeshBatch : public Renderable
	{
	public:
		MeshBatch(UINT nStreanmStride, PRIMITIVE_TYPE primitiveType, bool indexed, UINT initialCapacity = 1024, UINT growSize = 1024);

		~MeshBatch();

		virtual void	Render(Technique* pTech);

		void			Reset();

		template <class T>
		void			Add(const T* vertices, UINT vertexCount, const uint16* indices = NULL, UINT indexCount = 0);

		void			Add(const float* vertices, UINT vertexCount, const uint16* indices = NULL, UINT indexCount = 0);

	protected:
		void			Add(const void* vertices, size_t size, UINT vertexCount, const uint16* indices, UINT indexCount);

		bool			Resize(UINT capacity);

	protected:
		bool			m_bIndexed;
		UINT			m_nGrowSize;

		UINT			m_nCapacity;
		UINT			m_nVertexCapacity;
		UINT			m_nIndexCapacity;
		UINT			m_nStreanmStride;

		uint8*			m_pVerticesPtr;
		uint16*			m_pIndicesPtr;

		uint8*			m_pVerticesPtrBase;
		uint16*			m_pIndicesPtrBase;
	};

	template <class T>
	void MeshBatch::Add(const T* vertices, unsigned int vertexCount, const unsigned short* indices, unsigned int indexCount)
	{
		ASSERT(sizeof(T) == m_nStreanmStride);
		Add(vertices, sizeof(T), vertexCount, indices, indexCount);
	}
}



#endif // _MeshBatch_H_
