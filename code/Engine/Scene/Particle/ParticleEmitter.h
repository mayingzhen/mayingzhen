#ifndef PARTICLEEMITTER_H_
#define PARTICLEEMITTER_H_


namespace ma
{

	class Camera;
	class ParticleBatch;



	class ParticleEmitter : public RenderComponent
	{
		
	public:
		ParticleEmitter();

		~ParticleEmitter();

		DECL_OBJECT(ParticleEmitter)

		//virtual UINT		GetRenderableNumber() {return 1;}
		//virtual	Renderable*	GetRenderableByIndex(UINT index) {return m_pParticleBatch.get();}

		void				Load(const char* pParticleEmitPath);
	
		void				Start();

		void				Stop();

		virtual	void		Show(Camera* pCamera);

		virtual void		Serialize(Serializer& sl, const char* pszLable = "ParticleEmitter");


	private:
		RefPtr<ParticleBatch>	m_pParticleBatch;

		SubMaterial*				m_pMaterial;
	};

}

#endif
