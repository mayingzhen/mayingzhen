#include "BatchParticleRenderable.h"
#include "RenderSystem.h"
#include "IRenderDevice/TransientBuffer.h"

namespace ma
{
	struct ParticleSystem_InfoSortDescendingLess
	{
		ParticleSystem_InfoSortDescendingLess()
		{
		}

		bool operator()(const RefPtr<Renderable> a, const RefPtr<Renderable> b) const
		{
			SubMaterial* pA = a->GetMaterial();
			SubMaterial* pB = b->GetMaterial();
			//long i = pA->GetSortID() - pB->GetSortID();
			//if (i<0)
			//	return true;
			//else if(i>0)
			//	return false;

			long i = long(pA->GetShadingTechnqiue()->GetShaderProgram()) - long(pB->GetShadingTechnqiue()->GetShaderProgram());
			if (i<0)
				return true;
			else if(i>0)
				return false;

			i = long(pA) - long(pB);
			if (i< 0)
				return true;
			else if(i> 0)
				return false;

			i = a->m_pSubMeshData->m_nIndexStart - b->m_pSubMeshData->m_nIndexStart;
			if (i<0)
				return true;
			else if(i>0)
				return false;

			return false;
		}
	};

	BatchParticleRenderable::BatchParticleRenderable()
	{
	}

	bool BatchParticleRenderable::MerageRenderable(ParticleSystemRenderable* pSubMesh,VEC_RENDERABLE& arrMerageSubMesh, uint32 indexMerge)
	{
		ParticleSystemRenderable* pMerageSubMesh = (ParticleSystemRenderable*)arrMerageSubMesh[indexMerge];

		if (pSubMesh->m_pSubMeshData->m_nIndexCount <= 0)
			return false;

		ParallHardWareBuffer* pBuffer = GetRenderSystem()->GetRTParticleBuffer();

		BYTE* pIBPtr = pBuffer->GetIndexVideoMemoryBase();
		int nIndexStride = sizeof(uint16);

		if (pMerageSubMesh->m_pSubMeshData->m_nIndexStart + pMerageSubMesh->m_pSubMeshData->m_nIndexCount ==
			pSubMesh->m_pSubMeshData->m_nIndexStart)
		{
			pMerageSubMesh->m_pSubMeshData->m_nIndexCount += pSubMesh->m_pSubMeshData->m_nIndexCount;
		}
		else // µ÷Õû¶¥µãË³Ðò
		{
			BYTE* pSrcIB = pIBPtr + pSubMesh->m_pSubMeshData->m_nIndexStart * nIndexStride;
			uint32 nSrcSize = pSubMesh->m_pSubMeshData->m_nIndexCount * nIndexStride;
			uint32 nSrcStart = pSubMesh->m_pSubMeshData->m_nIndexStart;

			uint32 nMerageEnd =  pMerageSubMesh->m_pSubMeshData->m_nIndexStart + pMerageSubMesh->m_pSubMeshData->m_nIndexCount;
			BYTE* pDstIB = pIBPtr + nMerageEnd * nIndexStride; 

			ASSERT(nSrcStart > nMerageEnd);
			if (nSrcStart <= nMerageEnd)
				return false;

			uint32 nTempSize = (nSrcStart - nMerageEnd) * nIndexStride;

			m_vecTempIB.resize(nTempSize);

			memcpy(&m_vecTempIB[0],pDstIB,nTempSize);

			memcpy(pDstIB,pSrcIB,nSrcSize);

			memcpy(pDstIB + nSrcSize, &m_vecTempIB[0], nTempSize);

			pMerageSubMesh->m_pSubMeshData->m_nIndexCount += pSubMesh->m_pSubMeshData->m_nIndexCount;

			for (UINT m = indexMerge + 1; m < arrMerageSubMesh.size(); ++m)
			{
				arrMerageSubMesh[m]->m_pSubMeshData->m_nIndexStart += pSubMesh->m_pSubMeshData->m_nIndexCount;
			}
		}

		pSubMesh->m_pSubMeshData->m_nIndexStart = 0;
		pSubMesh->m_pSubMeshData->m_nIndexCount = 0;

		return true;
	}


	void BatchParticleRenderable::MerageRenderable(VEC_RENDERABLE& arrRenderable)
	{
		VEC_RENDERABLE arrMerageSubMesh;
		for (uint32 iSub = 0; iSub < arrRenderable.size(); ++iSub)
		{
			ParticleSystemRenderable* pSubMesh = (ParticleSystemRenderable*)arrRenderable[iSub];

			bool bCanMarge = false;

			for (uint32 jMerageSub = 0; jMerageSub < arrMerageSubMesh.size(); ++jMerageSub)
			{
				ParticleSystemRenderable* pMerageSubMesh = (ParticleSystemRenderable*)arrMerageSubMesh[jMerageSub];

				if ( pSubMesh->m_pSubMaterial != pMerageSubMesh->m_pSubMaterial)
					continue;

				bCanMarge = MerageRenderable(pSubMesh,arrMerageSubMesh,jMerageSub);	

				if (bCanMarge)
					break;
			}

			if (!bCanMarge)
				arrMerageSubMesh.push_back(pSubMesh);
		}

		arrRenderable = arrMerageSubMesh;
	}


	void BatchParticleRenderable::PrepareRender()
	{
		std::sort(m_arrRenderList.begin(), m_arrRenderList.end(), ParticleSystem_InfoSortDescendingLess());
		
		MerageRenderable(m_arrRenderList);

		GetRenderSystem()->UnLockParticleVideoMemory( GetRenderSystem()->GetPooIdRT() % RenderSystem::nNumParticleBuffer );
	}

}

