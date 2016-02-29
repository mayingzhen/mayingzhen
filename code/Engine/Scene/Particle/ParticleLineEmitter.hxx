#include "S3EngineCommon.h"
#include "ParticleLineEmitter.h"

namespace ma
{

CParticleLineEmitter::CParticleLineEmitter(void)
:m_vEnd(Vector3::ZERO)
{
	mMinIncrement = 0;
	mMaxIncrement = 0;
	mMaxDeviation = 0;

	mIncrementsLeft = true;
	mIncrement = 0;
	mFirst = true;
}

CParticleLineEmitter::~CParticleLineEmitter(void)
{
}

void CParticleLineEmitter::Reset()
{
    CParticleEmitter::Reset();

    mIncrementsLeft = true;
    mIncrement = 0;
    mFirst = true;
}

int CParticleLineEmitter::Emitt( Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles )
{
    if (!m_bEnabled)
        return 0;

	mParticles.clear();
	uint32 amount = this->GenConstantEmissionCount(now,timeSinceLastCall, nFinalMaxParticles);

	Vector3 vOffset = m_vEnd - mPos;
	if (mMaxDeviation > 0 && mFirst)
	{
		mPerpendicular = vOffset.crossProduct(Vector3(Math::RangeRandom(-1,1),
			Math::RangeRandom(-1,1),
			Math::RangeRandom(-1,1)));
		mPerpendicular.normalise();

		mLength = vOffset.length();
	}

	SParticle p;
	for (uint32 i=0; i<amount; ++i)
	{
		// If mMaxIncrement has been set, the particle emission follows a trajectory path along the line
		Real fraction = 0.0f;
		if (mMaxIncrement > 0.0f)
		{
			if (!mFirst)
			{
				mIncrement += (mMinIncrement + Math::UnitRandom() * (mMaxIncrement-mMinIncrement));
				if (mIncrement >= mLength)
				{
					mIncrementsLeft = false;
				}
				fraction = mIncrement / mLength;
			}
		}
		else
		{
			fraction = Math::UnitRandom();
		}

		if (mMaxDeviation > 0.0f && mIncrementsLeft)
		{
			if (!mFirst)
			{
				Vector3 basePosition = mPos + vOffset*fraction;
				p.pos = basePosition + mPerpendicular*(mMaxDeviation*Math::UnitRandom());
				p.prePos = p.startPos = basePosition;
			}
			else
			{
				p.prePos = p.startPos = p.pos = mPos;
			}
		}
		else
		{
			p.prePos = p.startPos  = p.pos = mPos + vOffset*fraction;
		}
		
		p.startTime = now-timeSinceLastCall;
		GenEmission(p);
		GenEmissionVelocity(p);

		if (!mIncrementsLeft)
		{
			p.endTime = 0.f;
		}
		mParticles.push_back(p);
		mFirst = false;
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

RefPtr<CParticleLineEmitter> CreateLineEmitter()
{
	return new CParticleLineEmitter;
}

}