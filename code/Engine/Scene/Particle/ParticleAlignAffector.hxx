#include "S3EngineCommon.h"
#include "ParticleAlignAffector.h"

namespace ma
{

ParticleAlignAffector::ParticleAlignAffector(void)
{
	mResize = false;
	mMinAddLength = 0;
	mMaxAddLength = 0;
}

ParticleAlignAffector::~ParticleAlignAffector(void)
{
}

// ---------------------------------------------------------------------
// Inherite from ParticleAffector
// ---------------------------------------------------------------------
void ParticleAlignAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if( !m_bEnabled )
		return;

	SParticle* pPreviousParticle = NULL;
	bool bFirst = true;
	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;
		if (pPreviousParticle == NULL)
		{
			pPreviousParticle = &p;
			p.size.height = p.startSize.height;
			continue;
		}

		Vector3 diff = pPreviousParticle->pos - p.pos;
		if (mResize)
		{
			if (bFirst)
			{
				bFirst = false;
				p.size.height = diff.length();
			}
			else
			{
				p.size.height = diff.length() + Math::Lerp<float>(mMinAddLength,mMaxAddLength, Randomizer::frand());
			}
		}

		diff.normalise();
		p.orientation.x = diff.x;
		p.orientation.y = diff.y;
		p.orientation.z = diff.z;

		pPreviousParticle = &p;
	}
}

// ---------------------------------------------------------------------
// Self
// ---------------------------------------------------------------------


RefPtr<ParticleAlignAffector> CreateAlignAffector()
{
	return new ParticleAlignAffector;
}

}