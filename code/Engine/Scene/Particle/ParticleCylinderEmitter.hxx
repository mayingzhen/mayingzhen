#include "S3EngineCommon.h"
#include "ParticleCylinderEmitter.h"

namespace ma
{
CParticleCylinderEmitter::CParticleCylinderEmitter()
{
}
CParticleCylinderEmitter::~CParticleCylinderEmitter(void)
{
}

int CParticleCylinderEmitter::Emitt( Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles )
{
    if (!m_bEnabled)
        return 0;

	mParticles.clear();
	uint32 amount = this->GenConstantEmissionCount(now,timeSinceLastCall, nFinalMaxParticles);
	SParticle p;
	for(uint32 i=0; i<amount; ++i)
	{
		Real x, y, z;
		// First we create a random point inside a bounding cylinder with a
		// radius and height of 1 (this is easy to do). The distance of the
		// point from 0,0,0 must be <= 1 (== 1 means on the surface and we
		// count this as inside, too).

		while (true)
		{
			/* ClearSpace not yet implemeted

			*/
			// three random values for one random point in 3D space
			x = Math::SymmetricRandom();
			y = Math::SymmetricRandom();
			z = Math::SymmetricRandom();

			// the distance of x,y from 0,0 is sqrt(x*x+y*y), but
			// as usual we can omit the sqrt(), since sqrt(1) == 1 and we
			// use the 1 as boundary. z is not taken into account, since
			// all values in the z-direction are inside the cylinder:
			if ( x*x + y*y <= 1)
			{
				break;          // found one valid point inside
			}
		}       

		// scale the found point to the cylinder's size and move it
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
 RefPtr<CParticleCylinderEmitter> CreateCylinderEmitter()
{
	return new CParticleCylinderEmitter();
}
}