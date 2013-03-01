#pragma once

//#include "VideoBufferManager.h"

namespace ma
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexDeclaration;

	class COMMON_API VertexStream 
	{
	public:
		VertexStream();
		~VertexStream();

		void SetDeclaration(VertexDeclaration* decl);
		VertexDeclaration* GetDeclaration() const;

		void Bind(int number, VertexBuffer* stream, int stride, int instance = 0);

		VertexBuffer* GetStream(int index) const;
		int GetStreamStride(int index) const;
		int GetStreamInstance(int index) const;

		void SetStart(int index) { mStart = index; }
		int GetStart() const { return mStart; }

		void SetCount(int size);
		int GetCount() const;

		VertexStream(const VertexStream & r);
		VertexStream & operator =(const VertexStream & r);

	public:
		int mStart;
		int mCount;
		VertexDeclaration* mDeclaration;
		int mStrides[MAX_VERTEX_STREAM];
		int mInstances[MAX_VERTEX_STREAM];
		VertexBuffer* mStreams[MAX_VERTEX_STREAM];
	};


	class COMMON_API IndexStream 
	{
	public:
						IndexStream();
						~IndexStream();

		void            Bind(IndexBuffer* stream, int start = 0);
		IndexBuffer*	GetStream() const;

		void			SetStart(int start)  { mStart = start; }
		int				GetStart() const { return mStart; }

		void            SetCount(int size);
		int             GetCount() const;

						IndexStream(const IndexStream & r);

		IndexStream &   operator =(const IndexStream & r);

	protected:
		int				mStart;
		int             mCount;
		IndexBuffer*	mStream;
	};

}
