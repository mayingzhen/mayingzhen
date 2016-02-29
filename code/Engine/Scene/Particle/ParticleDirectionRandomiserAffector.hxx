#include "S3EngineCommon.h"
#include "ParticleDirectionRandomiserAffector.h"

namespace ma
{

CParticleDirectionRandomiserAffector::CParticleDirectionRandomiserAffector(void)
{
	mRandomness = 1.0;
	mScope = 1.0;
	mKeepVelocity = false;
}

CParticleDirectionRandomiserAffector::~CParticleDirectionRandomiserAffector(void)
{
}

void CParticleDirectionRandomiserAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if( !m_bEnabled )
		return;

	Real length = 0;
	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;

		if (mScope > Math::UnitRandom())
		{
			if (!p.vector.isZeroLength())
			{
				if (mKeepVelocity)
				{
					length = p.vector.length();
				}

				p.vector += Vector3(Math::RangeRandom(-mRandomness, mRandomness) * timediff,
					Math::RangeRandom(-mRandomness, mRandomness) * timediff,
					Math::RangeRandom(-mRandomness, mRandomness) * timediff);

				if (mKeepVelocity)
				{
					p.vector *= length / p.vector.length();
				}
			}
		}
	}
}

RefPtr<CParticleDirectionRandomiserAffector> CreateDirectionRandomiserAffector()
{
	return new CParticleDirectionRandomiserAffector;
}

}