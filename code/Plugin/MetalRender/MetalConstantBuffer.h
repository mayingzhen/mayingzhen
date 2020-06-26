#pragma once

#import <Metal/Metal.h>

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

		id<MTLBuffer> GetMetalBuffer() {return m_pMetalBuffer[m_nCurFrame];}
        
        static void OnEndFrame();

		static void Clear();
        
    private:
        void UpdateFrame();

	private:
		/// Shadow data.
        //uint32_t m_nSize;


		id<MTLBuffer> m_pMetalBuffer[3];
        uint32_t m_nCurFrame;
        
        bool m_bApply = false;
        
	};

	RefPtr<MetalConstantBuffer> CreateConstantBuffer(const char* pszName, uint32_t nIndex, uint32_t size);

}
