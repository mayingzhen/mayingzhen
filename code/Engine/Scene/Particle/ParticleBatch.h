#ifndef _ParticleBatch_H_
#define _ParticleBatch_H_

#include "Particle.h"

namespace ma
{
	struct Particle;

	class ParticleBatch : public SpriteBatch
	{
	public:
		ParticleBatch();

		~ParticleBatch();

		void				Calculate(bool bWait);
		
		virtual	void		Render(Technique* pTech);

		void				SetUpdate(bool bUpadate) {m_bUpdate = bUpadate;}

		void				Load(const char* pParticleEmitPath);

		void				Start();

		void				Stop();

	protected:
		//  Update
		void				UpdateParticles();

		void				UpdateRenderable();

		void				EmitOnce(UINT particleCount);

		bool				isStarted() const;

		bool				isActive() const;

		void				LoadSpriteInfo(Properties* pSpriteProp,SpriteTextureInfo& info);

		void				LoadParticleEmitInfo(Properties* pProperties,ParticleEmitInfo& info);

		void				setSpriteFrameCoords(int nTextureWidth, int nTextureHeight);

	protected:
		UINT				m_nParticleCount;

		Particle*			m_pParticles;

		bool				m_bStarted;

		float				m_fTimeRunning;

		float*				m_pSpriteTextureCoords;

		SpriteTextureInfo	m_spriteTextureInfo;

		ParticleEmitInfo	m_particleEmitInfo;

		CriticalSection		m_csUpdate;	// 

		volatile bool		m_bUpdate;
	};

	DeclareRefPtr(ParticleBatch);

}





#endif