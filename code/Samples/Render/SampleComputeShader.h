#pragma once

namespace ma
{
	class Scene;

	class SampleComputeShader: public Sample
	{
	public:
		SampleComputeShader();

		virtual void	Load();

		virtual void	UnLoad() {}

		virtual void	PreRender();

	private:
		RefPtr<VertexBuffer> m_pInitVelBuffer;
		RefPtr<VertexBuffer> m_pBirthTimeBuffer;

		RefPtr<VertexBuffer> m_pPosBuffer;
		RefPtr<VertexBuffer> m_pVelBuffer;

		RefPtr<Technique> m_pCSTech;
	};
}



