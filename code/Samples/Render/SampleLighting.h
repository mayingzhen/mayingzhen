#ifndef _Sample_Lighting_H_
#define _Sample_Lighting_H_

namespace ma
{
	class SampleLighting : public Sample
	{
	public:
		SampleLighting();

		virtual void Load();

		virtual void UnLoad();

		virtual void Update();

		virtual void Render();

	private:
		std::vector<PointLight*> m_arrPointLight;
	};
}


#endif


