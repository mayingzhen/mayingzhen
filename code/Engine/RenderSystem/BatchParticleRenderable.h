#pragma once

namespace ma
{

	class BatchParticleRenderable : public BatchRenderable
	{
	public:
		
		BatchParticleRenderable();

		virtual void	PrepareRender();

	private:
		void MerageRenderable(VEC_RENDERABLE& vecRenderable);
		bool MerageRenderable(ParticleSystemRenderable* pSubMesh,VEC_RENDERABLE& arrMerageSubMesh, uint32 indexMerge);
			
	private:
		vector<BYTE> m_vecTempIB;
	};
}

