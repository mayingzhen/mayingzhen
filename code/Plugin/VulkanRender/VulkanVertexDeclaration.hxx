#include "VulkanVertexDeclaration.h"
#include "VulkanShaderProgram.h"

namespace ma
{
	//std::map<uint64, IVulkanInputLayout* > g_vertexDeclarationPool;

	VulkanVertexDeclaration::VulkanVertexDeclaration()
	{
		//m_pImpl = NULL;
	}

	VulkanVertexDeclaration::~VulkanVertexDeclaration()
	{
	}

	void VulkanVertexDeclaration::RT_StreamComplete()
	{
// 		ASSERT(m_pShader);
// 		if (m_pShader == NULL)
// 			return;
// 
// 		ASSERT(m_pImpl == NULL);
// 
// 		uint64 nHash = this->GetHash();
// 
// 		auto it = g_vertexDeclarationPool.find(nHash);
// 		if (it != g_vertexDeclarationPool.end())
// 		{
// 			m_pImpl = it->second;
// 		}
// 		else
// 		{
// 			Vulkan_INPUT_ELEMENT_DESC d3dve[MAX_ELEMENT];
// 			for (int i = 0; i < this->GetElementCount(); ++i)
// 			{
// 				const VertexElement& element = this->GetElement(i);
// 				d3dve[i].SemanticName = VulkanMapping::GetD3DDeclUsage(element.Usage);
// 				d3dve[i].SemanticIndex = element.UsageIndex;
// 				d3dve[i].Format = VulkanMapping::GetD3DDeclType(element.Type);
// 				d3dve[i].InputSlot = 0;
// 				d3dve[i].AlignedByteOffset = element.Offset;
// 				d3dve[i].InputSlotClass = Vulkan_INPUT_PER_VERTEX_DATA;
// 				d3dve[i].InstanceDataStepRate = 0;
// 			}
// 
// 			VulkanShaderProgram* pVulkanShader = (VulkanShaderProgram*)(m_pShader.get());
// 
// 			IVulkanInputLayout* pVulkanVertexDecl = NULL;
// 			GetVulkanDxDevive()->CreateInputLayout(&d3dve[0], GetElementCount(), pVulkanShader->GetByteVSCode(),
// 				pVulkanShader->GetByteVSCodeSize(), &pVulkanVertexDecl);
// 			ASSERT(pVulkanVertexDecl);
// 			if (pVulkanVertexDecl == NULL)
// 			{
// 				LogError("Failed to create input layout");
// 			}
// 
// 			g_vertexDeclarationPool.insert(std::make_pair(nHash, pVulkanVertexDecl));
// 
// 			m_pImpl = pVulkanVertexDecl;
// 		}
	}

	void VulkanVertexDeclaration::Clear()
	{
// 		for (auto it = g_vertexDeclarationPool.begin(); it != g_vertexDeclarationPool.end(); ++it)
// 		{
// 			SAFE_RELEASE(it->second);
// 		}
// 		g_vertexDeclarationPool.clear();
	}
}
