#pragma once

namespace ma
{
	class PrefilterCube
	{
	public:
		void Init(const char* in_file);
		
		void Render();
		
		void Save(const char* out_file);

		Texture* GetOutTexture() { return m_pOutTexture.get(); }

		void GenIntegrateBRDF(const char* out_file);

	private:
		RefPtr<Texture> m_pInTexture;

		RefPtr<Texture> m_pOutTexture;

		RefPtr<RenderPass> m_pRendePass[6][12];

		RefPtr<Technique> m_pTechnique[6][12];

		uint32_t m_nOutMipmaps = 1;
	};
}
