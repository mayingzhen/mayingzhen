
#pragma once

namespace ma
{

	/// Hardware constant buffer.
	class VulkanConstantBuffer : public ConstantBuffer
	{
	public:
		VulkanConstantBuffer();
		virtual ~VulkanConstantBuffer();

		/// Release buffer.
		virtual void Release();

		/// Set size and create GPU-side buffer. Return true on success.
		//bool SetSize(unsigned size);

		//UINT GetSize() { return m_nSize; }

		/// Set a generic parameter and mark buffer dirty.
		void SetParameter(unsigned offset, unsigned size, const void* data);

		/// Apply to GPU.
		void Apply();

		/// Return size.
		//unsigned GetSize() const { return m_shadowData.size(); }

		/// Return whether has unapplied data.
		bool IsDirty() const { return m_bDirty; }

		//IVulkanBuffer* GetVulkanBuffer() {return m_pVulkanBuffer;}

		bool Create();

		static void Clear();

	public:
		/// Shadow data.
		//vector<BYTE> m_shadowData;

		/// Dirty flag.
		bool m_bDirty;

		//IVulkanBuffer* m_pVulkanBuffer;
		VkBuffer m_buf;
		VkDeviceMemory m_mem;
		VkDescriptorBufferInfo m_descriptor;

		BYTE* m_mapped = nullptr;
	};

	RefPtr<VulkanConstantBuffer> CreateConstantBuffer(ShaderType type, unsigned index, unsigned size);

}
