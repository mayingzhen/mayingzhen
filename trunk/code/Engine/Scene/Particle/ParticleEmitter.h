#ifndef PARTICLEEMITTER_H_
#define PARTICLEEMITTER_H_


namespace ma
{

	class Camera;
	class ParticleBatch;



	class RENDER_API ParticleEmitter : public RenderComponent
	{
		DECL_OBJECT(ParticleEmitter)

	public:
		ParticleEmitter(SceneNode* pGameObj);

		~ParticleEmitter();

		//virtual UINT		GetRenderableNumber() {return 1;}

		//virtual	Renderable*	GetRenderableByIndex(UINT index) {return m_pParticleBatch.get();}

		void				Load(const char* pParticleEmitPath);
	
		void				Start();

		void				Stop();

		//virtual Material*	GetMaterial();

		virtual	void		Show(Camera* pCamera);

		virtual void		Serialize(Serializer& sl, const char* pszLable = "ParticleEmitter");


	private:
		RefPtr<ParticleBatch>	m_pParticleBatch;

		SubMaterial*				m_pMaterial;
	};

	DeclareRefPtr(ParticleEmitter);
}

#endif
