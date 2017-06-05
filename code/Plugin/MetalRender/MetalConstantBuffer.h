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
		bool SetSize(unsigned size);

		/// Set a generic parameter and mark buffer dirty.
		void SetParameter(unsigned offset, unsigned size, const void* data);

		/// Set a Vector3 array parameter and mark buffer dirty.
		void SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data);

		/// Apply to GPU.
		void Apply();

		/// Return size.
		unsigned GetSize() const { return m_shadowData.size(); }

		/// Return whether has unapplied data.
		bool IsDirty() const { return m_bDirty; }

		id<MTLBuffer> GetMetalBuffer() {return m_pMetalBuffer;}

		static void Clear();

	private:
		/// Shadow data.
		vector<BYTE> m_shadowData;

		/// Dirty flag.
		bool m_bDirty;

		id<MTLBuffer> m_pMetalBuffer;
	};

	RefPtr<ConstantBuffer> CreateConstantBuffer(ShaderType type, unsigned index, unsigned size);

}
