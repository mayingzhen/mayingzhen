#pragma once
#include "ParticleAffector.h"

namespace ma
{

class CParticleLineDriftAffector : public CParticleAffector
{
public:
	CParticleLineDriftAffector(void);
	~CParticleLineDriftAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleAffector
	// ---------------------------------------------------------------------
public:
    virtual void Reset();

	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	//! Get emitter type
	virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_LINE_DRIFT; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:
    // 距离线段的最大偏移值,这个值会再乘以偏移强度Drift
	void SetMaxDeviation(Real maxDeviation){mMaxDeviation = maxDeviation;}
	Real GetMaxDeviation() const{return mMaxDeviation;}

    // 线段的终点坐标
	void SetEnd(const Vector3& end){mEnd = end;}
	const Vector3& GetEnd() const{return mEnd;}

    // 设置更新时间间隔（秒）,默认0.1，为了节约效率不需要每帧都更新
	void SetTimeStep(Real timeStep){mTimeStep = timeStep;}
	Real GetTimeStep() const{return mTimeStep;}

    // 偏移强度[0.0~1.0]，为0时偏移无效
	void SetDrift(Real drift){mDrift = drift;mOneMinusDrift = 1.f - drift;}
	Real GetDrift() const{return mDrift;}
private:
	Real mMaxDeviation;
	Vector3 mEnd;
	Real mTimeStep;
	Real mDrift;

	Real mOneMinusDrift;
	bool mUpdate;
	bool mFirst;
	Real mTimeSinceLastUpdate;
};

RefPtr<CParticleLineDriftAffector> CreateLineDriftAffector();
}
