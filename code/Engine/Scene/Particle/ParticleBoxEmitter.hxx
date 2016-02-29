#include "S3EngineCommon.h"
#include "ParticleBoxEmitter.h"

namespace ma
{

CParticleBoxEmitter::CParticleBoxEmitter()
{
}

CParticleBoxEmitter::~CParticleBoxEmitter(void)
{
}

// ---------------------------------------------------------------------
// Inherite from CParticleEmitter
// ---------------------------------------------------------------------
int CParticleBoxEmitter::Emitt( Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles )
{
    if (!m_bEnabled)
        return 0;

	mParticles.clear();
	uint32 amount = this->GenConstantEmissionCount(now,timeSinceLastCall, nFinalMaxParticles);
		
	SParticle p;
	for (uint32 i=0; i<amount; ++i)
	{
		p.prePos = p.startPos  = p.pos = mPos + mXRange*Math::SymmetricRandom()+ mYRange*Math::SymmetricRandom() + mZRange*Math::SymmetricRandom();

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

// ---------------------------------------------------------------------
// Self
// ---------------------------------------------------------------------



// ---------------------------------------------------------------------
// Global Function
// ---------------------------------------------------------------------

RefPtr<CParticleBoxEmitter> CreateBoxEmitter()
{
	return new CParticleBoxEmitter();
}
}