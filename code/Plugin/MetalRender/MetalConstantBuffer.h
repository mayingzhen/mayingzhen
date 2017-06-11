#pragma once


namespace ma
{

	/// Hardware constant buffer.
	class ConstantBuffer : public Referenced
	{
	public:
		ConstantBuffer();
		virtual ~ConstantBuffer();

		/// Release buffer.
		virtual void Release();

		/// Set size and create GPU-side buffer. Return true on success.
		bool SetSize(UINT size,UINT nIndex);

		/// Set a generic parameter and mark buffer dirty.
		void SetParameter(unsigned offset, unsigned size, const void* data);

		/// Set a Vector3 array parameter and mark buffer dirty.
		void SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data);

		/// Apply to GPU.
		void Apply(id<MTLRenderCommandEncoder> renderEncoder,bool ps);

		/// Return size.
		//unsigned GetSize() const { return m_shadowData.size(); }

		/// Return whether has unapplied data.
		bool IsDirty() const { return m_bDirty; }

		id<MTLBuffer> GetMetalBuffer() {return m_pMetalBuffer;}

		static void Clear();

	private:
		/// Shadow data.
		void* m_shadowData;
        UINT m_nSize;

		/// Dirty flag.
		bool m_bDirty;

		id<MTLBuffer> m_pMetalBuffer;
        
        UINT m_nIndex;
        
	};

	RefPtr<ConstantBuffer> CreateConstantBuffer(const char* pszName, UINT nIndex, UINT size);

}
