#pragma once


namespace ma
{

	/// Hardware constant buffer.
	class MetalConstantBuffer : public ConstantBuffer
	{
	public:
		MetalConstantBuffer();
		virtual ~MetalConstantBuffer();
        
        /// Set size and create GPU-side buffer. Return true on success.
        bool Create();

		/// Release buffer.
		virtual void Release();

		/// Set a generic parameter and mark buffer dirty.
		void SetParameter(unsigned offset, unsigned size, const void* data);

		/// Apply to GPU.
		void Apply(id<MTLRenderCommandEncoder> renderEncoder,bool ps);

		/// Return size.
		//unsigned GetSize() const { return m_shadowData.size(); }

		/// Return whether has unapplied data.
		bool IsDirty() const { return m_bDirty; }

		id<MTLBuffer> GetMetalBuffer() {return m_pMetalBuffer[m_nCurFrame];}
        

        static void OnEndFrame();

		static void Clear();
        
    private:
        void UpdateFrame();

	private:
		/// Shadow data.
        //UINT m_nSize;

		/// Dirty flag.
		bool m_bDirty;

		id<MTLBuffer> m_pMetalBuffer[3];
        UINT m_nCurFrame;
        
	};

	RefPtr<MetalConstantBuffer> CreateConstantBuffer(const char* pszName, UINT nIndex, UINT size);

}
