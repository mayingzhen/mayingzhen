#ifndef _MeshBatch_H_
#define _MeshBatch_H_

#include "Renderable.h"

namespace ma
{
	class Effect;

	class ENGINE_API MeshBatch : public Renderable
	{
	public:
		MeshBatch(VertexDeclaration* vertexFormat, PRIMITIVE_TYPE primitiveType, Effect* pEffect, bool indexed, UINT initialCapacity = 1024, UINT growSize = 1024);

		~MeshBatch();

		void start();

		template <class T>
		void add(const T* vertices, UINT vertexCount, const unsigned short* indices = NULL, UINT indexCount = 0);

		void add(const float* vertices, UINT vertexCount, const unsigned short* indices = NULL, UINT indexCount = 0);

		void finish();

	private:

		void add(const void* vertices, size_t size, UINT vertexCount, const unsigned short* indices, UINT indexCount);

		bool resize(UINT capacity);

	private:
		bool			m_bIndexed;
		UINT			m_nGrowSize;

		UINT			m_nCapacity;
		UINT			m_nVertexCapacity;
		UINT			m_nIndexCapacity;

		unsigned char*	m_pVerticesPtr;
		unsigned short* m_pIndicesPtr;
	};

}

#include "MeshBatch.inl"

#endif // _MeshBatch_H_
