#include "S3EngineCommon.h"
#include "ParticleRingEmitter.h"

namespace ma
{


ParticleRingEmitter::ParticleRingEmitter()
: mInnerRatio(0.5f, 0.5f)
{
}

ParticleRingEmitter::~ParticleRingEmitter(void)
{
}

int ParticleRingEmitter::Emitt( Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles )
{
    if (!m_bEnabled)
        return 0;

	mParticles.clear();
	uint32 amount = this->GenConstantEmissionCount(now,timeSinceLastCall, nFinalMaxParticles);

	SParticle p;
	for(uint32 i=0; i<amount; ++i)
	{
		Real a, b, x, y, z;

		// create a random angle from 0 .. PI*2
		Radian alpha ( Math::RangeRandom(0,Math::TWO_PI) );

		// create two random radius values that are bigger than the inner size
		a = Math::RangeRandom(mInnerRatio.x,1.0);
		b = Math::RangeRandom(mInnerRatio.y,1.0);

		// with a and b we have defined a random ellipse inside the inner
		// ellipse and the outer circle (radius 1.0)
		// with alpha, and a and b we select a random point on this ellipse
		// and calculate it's coordinates
		x = a * Math::Sin(alpha);
		y = b * Math::Cos(alpha);
		// the height is simple -1 to 1
		z = Math::SymmetricRandom();     

		// scale the found point to the ring's size and move it
		// relatively to the center of the emitter point
		p.prePos = p.startPos = p.pos = mPos + mXRange*x + mYRange*y + mZRange*z;

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

RefPtr<ParticleRingEmitter> CreateRingEmitter()
{
	return new ParticleRingEmitter();
}
}