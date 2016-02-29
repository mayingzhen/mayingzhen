#include "S3EngineCommon.h"
#include "ParticleColourFaderAffector.h"

namespace ma
{

CParticleColourFaderAffector::CParticleColourFaderAffector()
:mColourAdder(Vector4::ZERO)
{
}

CParticleColourFaderAffector::~CParticleColourFaderAffector(void)
{
}

void CParticleColourFaderAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if( !m_bEnabled )
		return;

	Vector4 delta = mColourAdder*timediff;
	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;
		p.color = p.color+ (ColourValue&)delta;
		p.color.saturate();
	}
}

RefPtr<CParticleColourFaderAffector> CreateColourFaderAffector()
{
	return new CParticleColourFaderAffector();
}
}