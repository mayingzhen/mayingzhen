#ifndef  _VertexStream__H__
#define  _VertexStream__H__

#define VertexDeclarationPtr VertexDeclaration*
#define VertexBufferPtr VertexBuffer*
#define IndexBufferPtr IndexBuffer*

namespace ma
{	
	class ENGINE_API VertexStream : public RefCountObject
	{
	public:
		VertexStream();
		~VertexStream();

		void SetDeclaration(VertexDeclarationPtr decl);
		VertexDeclarationPtr GetDeclaration() const;

		void Bind(int number, VertexBufferPtr stream, int stride, int instance = 0);

		VertexBufferPtr GetStream(int index) const;
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
		VertexDeclarationPtr mDeclaration;
		int mStrides[MAX_VERTEX_STREAM];
		int mInstances[MAX_VERTEX_STREAM];
		VertexBufferPtr mStreams[MAX_VERTEX_STREAM];
	};


	class ENGINE_API IndexStream : public RefCountObject
	{
	public:
						IndexStream();
						~IndexStream();

		void            Bind(IndexBufferPtr stream, int start = 0);
		IndexBufferPtr  GetStream() const;

		void			SetStart(int start)  { mStart = start; }
		int				GetStart() const { return mStart; }

		void            SetCount(int size);
		int             GetCount() const;

						IndexStream(const IndexStream & r);

		IndexStream &   operator =(const IndexStream & r);

	protected:
		int				mStart;
		int             mCount;
		IndexBufferPtr  mStream;
	};

}

#endif
