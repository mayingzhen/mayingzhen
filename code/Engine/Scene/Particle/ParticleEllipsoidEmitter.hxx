#include "S3EngineCommon.h"
#include "ParticleEllipsoidEmitter.h"

namespace ma
{

ParticleEllipsoidEmitter::ParticleEllipsoidEmitter()
{
}

ParticleEllipsoidEmitter::~ParticleEllipsoidEmitter(void)
{
}

int ParticleEllipsoidEmitter::Emitt( Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles )
{
    if (!m_bEnabled)
        return 0;

	mParticles.clear();
	uint32 amount = this->GenConstantEmissionCount(now,timeSinceLastCall, nFinalMaxParticles);

	SParticle p;
	for(uint32 i=0; i<amount; ++i)
	{
		Real x, y, z;
		// First we create a random point inside a bounding sphere with a
		// radius of 1 (this is easy to do). The distance of the point from
		// 0,0,0 must be <= 1 (== 1 means on the surface and we count this as
		// inside, too).

		while (true)
		{
			// three random values for one random point in 3D space

			x = Math::SymmetricRandom();
			y = Math::SymmetricRandom();
			z = Math::SymmetricRandom();

			// the distance of x,y,z from 0,0,0 is sqrt(x*x+y*y+z*z), but
			// as usual we can omit the sqrt(), since sqrt(1) == 1 and we
			// use the 1 as boundary:
			if ( x*x + y*y + z*z <= 1)
			{
				break;          // found one valid point inside
			}
		}    
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

RefPtr<ParticleEllipsoidEmitter> CreateEllipsoidEmitter()
{
	return new ParticleEllipsoidEmitter();
}
}