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


		RefPtr<SubMaterial> m_pMaterial;

		
	};
}



