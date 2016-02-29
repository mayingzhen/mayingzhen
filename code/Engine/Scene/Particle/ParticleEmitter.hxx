#include "ParticleEmitter.h"

namespace ma
{

CParticleEmitter::CParticleEmitter()
:mPos(Vector3::ZERO), mMinStartSize(FloatSize(1.f, 1.f)), mMaxStartSize(FloatSize(1.f, 1.f)),
mEmissionRate(10.f),
mMinStartColor(ColourValue::White), mMaxStartColor(ColourValue::White),
mMinLifeTime(2), mMaxLifeTime(4),
mMaxAngleDegrees(Vec3i(0,0,0)), mRemainder(0.f)
{
    m_bEnabled = true;
	mMinVelocity = mMaxVelocity = 1.f;
	this->SetDirection(Vector3::UNIT_Z);

	mStartTime = mStartTimeRemain = 0;
	mInternalEnabled = true;
	mDuration = mDurationRemain = 0;
	mRepeatDelay = mRepeatDelayRemain = 0;
    m_bLodEnabled = true;
}

CParticleEmitter::~CParticleEmitter(void)
{
}

void CParticleEmitter::RegisterAttribute()
{
	REF_ACCESSOR_ATTRIBUTE(CParticleEmitter, "Direction", GetDirection, SetDirection, Vector3, Vector3::UNIT_Z, AM_DEFAULT);
	ACCESSOR_ATTRIBUTE(CParticleEmitter, "MinLifeTime", GetMinLifeTime, SetMinLifeTime, float, 2, AM_DEFAULT);
	ACCESSOR_ATTRIBUTE(CParticleEmitter, "MaxLifeTime", GetMaxLifeTime, SetMaxLifeTime, float, 4, AM_DEFAULT);
	ACCESSOR_ATTRIBUTE(CParticleEmitter, "MinVelocity", GetMinVelocity, SetMinVelocity, float, 1, AM_DEFAULT);
	ACCESSOR_ATTRIBUTE(CParticleEmitter, "MaxVelocity", GetMaxVelocity, SetMaxVelocity, float, 1, AM_DEFAULT);
	REF_ACCESSOR_ATTRIBUTE(CParticleEmitter, "MaxAngleDegrees", GetMaxAngleDegrees, SetMaxAngleDegrees, Vec3i, Vec3i(0,0,0), AM_DEFAULT);
}

void CParticleEmitter::Reset()
{
    mRemainder = 0.f;
    mInternalEnabled = true;
    this->SetStartTime(mStartTime);
    this->SetDuration(mDuration);
    this->SetRepeatDelay(mRepeatDelay);
}

void CParticleEmitter::GenEmissionVelocity( IN OUT SParticle& p )
{
	p.vector = mDirection*(mMinVelocity + Randomizer::frand()*(mMaxVelocity-mMinVelocity));
    {
		Vector3 tgt = p.vector;
        if (mMaxAngleDegrees.z)
		    tgt.rotateXYBy(Radian(Degree((Real)(Randomizer::rand()%(mMaxAngleDegrees.z*2) - mMaxAngleDegrees.z))));

        if(mMaxAngleDegrees.x)
		    tgt.rotateYZBy(Radian(Degree((Real)(Randomizer::rand()%(mMaxAngleDegrees.x*2) - mMaxAngleDegrees.x))));

        if(mMaxAngleDegrees.y)
		    tgt.rotateXZBy(Radian(Degree((Real)(Randomizer::rand()%(mMaxAngleDegrees.y*2) - mMaxAngleDegrees.y))));

		p.vector = tgt;
	}

	p.startVector = p.vector;
}

void CParticleEmitter::GenEmission( IN OUT SParticle& p )
{
	if (mMaxLifeTime - mMinLifeTime == 0)
		p.endTime = p.startTime + mMinLifeTime;
	else
		p.endTime = p.startTime + Math::Lerp(mMinLifeTime, mMaxLifeTime, Randomizer::frand());

	p.color.r = Math::Lerp<Real>(mMinStartColor.r,	mMaxStartColor.r, Randomizer::frand());
	p.color.g = Math::Lerp<Real>(mMinStartColor.g,	mMaxStartColor.g, Randomizer::frand());
	p.color.b = Math::Lerp<Real>(mMinStartColor.b,	mMaxStartColor.b, Randomizer::frand());
	p.color.a = Math::Lerp<Real>(mMinStartColor.a,	mMaxStartColor.a, Randomizer::frand());

	p.startColor = p.color;
	if (mMinStartSize==mMaxStartSize)
		p.startSize = mMinStartSize;
	else
	{
		float fPercent = Randomizer::frand();
		p.startSize.width = Math::Lerp<float>(mMinStartSize.width,mMaxStartSize.width, fPercent);
		p.startSize.height = Math::Lerp<float>(mMinStartSize.height,mMaxStartSize.height, fPercent);
	}
	p.size = p.startSize;
}

uint32 CParticleEmitter::GenConstantEmissionCount( Real now, Real timeElapsed, uint32 nFinalMaxParticles )
{
	if (mInternalEnabled)
	{
		// Keep fractions, otherwise a high frame rate will result in zero emissions!
        if (m_bLodEnabled)
            mRemainder += mEmissionRate*g_fParticlePercent*timeElapsed;
        else
            mRemainder += mEmissionRate*timeElapsed;
		
		uint32 intRequest = (uint32)mRemainder;
		mRemainder -= intRequest;

		if (mDuration)
		{
			mDurationRemain -= timeElapsed;
		}

		// burst
		for (DEQ_BURST::iterator iter = mBursts.begin();iter != mBursts.end();++iter)
		{
			BURST* burst = (*iter).get();
			if (burst->bToEmit && mDuration - mDurationRemain >= burst->fTime)
			{
                if (m_bLodEnabled)
					intRequest += burst->nNumParticles>0?__max(1, uint32(burst->nNumParticles*g_fParticlePercent)):0;
                else
                    intRequest += uint32(burst->nNumParticles);
				burst->bToEmit = false;
			}
		}

		// check duration
		if (mDuration)
		{
			if (mDurationRemain <= 0)
			{
				// Disable, duration is out (takes effect next time)
				SetInternalEnable(false);
			}
		}

		return __min(intRequest, nFinalMaxParticles);
	}
	else
	{
		// Check repeat
		if (mRepeatDelay)
		{
			mRepeatDelayRemain -= timeElapsed;
			if (mRepeatDelayRemain <= 0)
			{
				// Enable, repeat delay is out (takes effect next time)
				SetInternalEnable(true);
			}
		}

		if(mStartTimeRemain)
		{
			mStartTimeRemain -= timeElapsed;
			if(mStartTimeRemain <= 0)
			{
				SetInternalEnable(true);
				mStartTimeRemain = 0;
			}
		}
		return 0;
	}
}

CParticleEmitter::BURST* CParticleEmitter::AddBurst(Real time, uint32 nNumParticles)
{
	RefPtr<BURST> info = new BURST;
	info->fTime = time;
	info->bToEmit = true;
	info->nNumParticles = nNumParticles;
	info->pEmitter = this;
	mBursts.push_back(info);
	return mBursts.back().get();
}

uint32 CParticleEmitter::GetNumBursts() const
{
	return mBursts.size();
}

void CParticleEmitter::ClearBursts()
{
	mBursts.clear();
}

void CParticleEmitter::RemoveBurstByIndex(uint32 nIndex)
{
	mBursts.erase(mBursts.begin()+nIndex);
}

CParticleEmitter::BURST* CParticleEmitter::GetBurstByIndex(uint32 nIndex)
{
	if (nIndex >= mBursts.size())
	{
		return NULL;
	}

	return mBursts[nIndex].get();
}

void CParticleEmitter::RemoveBurst( BURST* pBurst )
{
	for (DEQ_BURST::iterator iter = mBursts.begin();iter != mBursts.end();++iter)
	{
		if ((*iter) == pBurst)
		{
			mBursts.erase(iter);
			return;
		}
	}
}

void CParticleEmitter::SetStartTime( Real startTime )
{
	if(startTime > 0)
		this->SetInternalEnable(false);

	mStartTimeRemain = mStartTime = startTime;
}

void CParticleEmitter::SetInternalEnable( bool bEnable )
{
	mInternalEnabled = bEnable;
	// Reset duration & repeat
	InitDurationRepeat();
}

void CParticleEmitter::InitDurationRepeat( void )
{
	if (mInternalEnabled)
	{
		mDurationRemain = mDuration;
		for (DEQ_BURST::iterator iter = mBursts.begin();iter != mBursts.end();++iter)
		{
			(*iter)->bToEmit = true;
		}
	}
	else
	{
		// Reset repeat
		mRepeatDelayRemain = mRepeatDelay;
	}
}

void CParticleEmitter::SetDuration( Real duration )
{
	mDuration = duration;
	InitDurationRepeat();
}

void CParticleEmitter::SetRepeatDelay( Real duration )
{
	mRepeatDelay = duration;
	InitDurationRepeat();
}

bool CParticleEmitter::GetEmitting(float fNow) const
{
    if (mRepeatDelay > 0 || mDuration == 0)
    {
        return true;
    }


    if (fNow <= mStartTime+mDuration)
    {
        return true;
    }

    return false;
}

}