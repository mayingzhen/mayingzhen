#ifndef _MeshBatch_H_
#define _MeshBatch_H_

#include "Renderable.h"

namespace ma
{
	class MeshBatch : public Renderable
	{
	public:
		MeshBatch(uint32_t nStreanmStride, PRIMITIVE_TYPE primitiveType, bool indexed, uint32_t initialCapacity = 1024, uint32_t growSize = 1024);

		~MeshBatch();

		virtual void	Render(Technique* pTech);

		void			Reset();

		template <class T>
		void			Add(const T* vertices, uint32_t vertexCount, const uint16_t* indices = NULL, uint32_t indexCount = 0);

		void			Add(const float* vertices, uint32_t vertexCount, const uint16_t* indices = NULL, uint32_t indexCount = 0);

	protected:
		void			Add(const void* vertices, size_t size, uint32_t vertexCount, const uint16_t* indices, uint32_t indexCount);

		bool			Resize(uint32_t capacity);

	protected:
		bool			m_bIndexed;
		uint32_t		m_nGrowSize;

		uint32_t		m_nCapacity;
		uint32_t		m_nVertexCapacity;
		uint32_t		m_nIndexCapacity;
		uint32_t		m_nStreanmStride;

		uint8_t*		m_pVerticesPtr;
		uint16_t*		m_pIndicesPtr;

		uint8_t*		m_pVerticesPtrBase;
		uint16_t*		m_pIndicesPtrBase;
	};

	template <class T>
	void MeshBatch::Add(const T* vertices, unsigned int vertexCount, const unsigned short* indices, unsigned int indexCount)
	{
		ASSERT(sizeof(T) == m_nStreanmStride);
		Add(vertices, sizeof(T), vertexCount, indices, indexCount);
	}
}



#endif // _MeshBatch_H_
