#include "S3EngineCommon.h"
#include "ParticleDampingAffector.h"

namespace ma
{

ParticleDampingAffector::ParticleDampingAffector(void)
: m_fDampingSpeed(1.f)
{
	mStartTime = 0.f;
	mDuration = 999999.f;
}

ParticleDampingAffector::~ParticleDampingAffector(void)
{
}

void ParticleDampingAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
    if( !m_bEnabled )
        return;

	if (now < mStartTime || now > mStartTime+mDuration)
		return;

    for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
    {
        SParticle& p = *iter;
        float fSrcLen = p.vector.length();
        if (fSrcLen == 0)
        {
            continue;
        }

        float fLen = __max(0, fSrcLen - m_fDampingSpeed*timediff);
        if (fLen == 0)
        {
            p.vector = Vector3::ZERO;
            continue;
        }

        p.vector = p.vector.normalisedCopy()*fLen;
    }
}

RefPtr<ParticleDampingAffector> CreateDampingAffector()
{
    return new ParticleDampingAffector;
}

}