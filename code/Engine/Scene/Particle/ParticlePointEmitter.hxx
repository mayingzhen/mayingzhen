#include "ParticlePointEmitter.h"

namespace ma
{

CParticlePointEmitter::CParticlePointEmitter()
{
}

CParticlePointEmitter::~CParticlePointEmitter(void)
{
}

void CParticlePointEmitter::RegisterAttribute()
{
	REGISTER_OBJECT(CParticlePointEmitter,CreatePointEmitter); 

	COPY_BASE_ATTRIBUTES(CParticlePointEmitter,CParticleEmitter);
}

int CParticlePointEmitter::Emitt( Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles )
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

 RefPtr<CParticlePointEmitter> CreatePointEmitter()
{
	return new CParticlePointEmitter();
}
}