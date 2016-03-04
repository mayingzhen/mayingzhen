#include "ParticlePointEmitter.h"

namespace ma
{

ParticlePointEmitter::ParticlePointEmitter()
{
}

ParticlePointEmitter::~ParticlePointEmitter(void)
{
}

void ParticlePointEmitter::RegisterAttribute()
{
	REGISTER_OBJECT(ParticlePointEmitter,CreatePointEmitter); 

	COPY_BASE_ATTRIBUTES(ParticlePointEmitter,ParticleEmitter);
}

int ParticlePointEmitter::Emitt( Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles )
{
    if (!m_bEnabled)
        return 0;

	mParticles.clear();
	uint32 amount = this->GenConstantEmissionCount(now,timeSinceLastCall, nFinalMaxParticles);

	SParticle p;
	for (uint32 i=0; i<amount; ++i)
	{
		p.prePos = p.startPos = p.pos = mPos;

		p.startTime = now-timeSinceLastCall;
		GenEmission(p);
		GenEmissionVelocity(p);

		mParticles.push_back(p);
	}
	
	if (mParticles.empty())
	{
		outArray = NULL;
	}
	else
	{
		outArray = &mParticles[0];
	}

	return mParticles.size();
}

 RefPtr<ParticlePointEmitter> CreatePointEmitter()
{
	return new ParticlePointEmitter();
}
}