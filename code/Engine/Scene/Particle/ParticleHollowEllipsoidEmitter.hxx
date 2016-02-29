#include "S3EngineCommon.h"
#include "ParticleHollowEllipsoidEmitter.h"

namespace ma
{

CParticleHollowEllipsoidEmitter::CParticleHollowEllipsoidEmitter(void)
: mInnerRatio(0.5,0.5,0.5)
{

}

CParticleHollowEllipsoidEmitter::~CParticleHollowEllipsoidEmitter(void)
{
}

int CParticleHollowEllipsoidEmitter::Emitt( Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles )
{
    if (!m_bEnabled)
        return 0;

	mParticles.clear();
	uint32 amount = this->GenConstantEmissionCount(now,timeSinceLastCall, nFinalMaxParticles);
		
	SParticle p;
	for(uint32 i=0; i<amount; ++i)
	{
		Real a, b, c, x, y, z;
		
		// create two random angles alpha and beta
		// with these two angles, we are able to select any point on an
		// ellipsoid's surface
		Radian alpha ( Math::RangeRandom(0,Math::TWO_PI) );
		Radian beta  ( Math::RangeRandom(0,Math::PI) );

		// create three random radius values that are bigger than the inner
		// size, but smaller/equal than/to the outer size 1.0 (inner size is
		// between 0 and 1)
		a = Math::RangeRandom(mInnerRatio.x,1.0);
		b = Math::RangeRandom(mInnerRatio.y,1.0);
		c = Math::RangeRandom(mInnerRatio.z,1.0);

		// with a,b,c we have defined a random ellipsoid between the inner
		// ellipsoid and the outer sphere (radius 1.0)
		// with alpha and beta we select on point on this random ellipsoid
		// and calculate the 3D coordinates of this point
		Real sinbeta ( Math::Sin(beta) );
		x = a * Math::Cos(alpha) * sinbeta;
		y = b * Math::Sin(alpha) * sinbeta;
		z = c * Math::Cos(beta);

		// scale the found point to the ellipsoid's size and move it
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

RefPtr<CParticleHollowEllipsoidEmitter> CreateHollowEllipsoidEmitter()
{
	return new CParticleHollowEllipsoidEmitter;
}

}