#pragma once
#include "ParticleAffector.h"

namespace ma
{

class  CParticleScaleInterpolatorAffector : public CParticleAffector
{
	#define MAX_STAGES 6
public:
	CParticleScaleInterpolatorAffector(void);
	~CParticleScaleInterpolatorAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleAffector
	// ---------------------------------------------------------------------
public:
	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	//! Get emitter type
	virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_SCALE_INTERPOLATOR; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
    // 第index阶段的缩放（长宽,startSize乘以当前缩放值）
	void SetScaleAdjust(size_t index, const Vector2& scale){ASSERT(index < MAX_STAGES); mScaleAdj[index] = scale;}
	const Vector2& GetScaleAdjust(size_t index) const{ASSERT(index < MAX_STAGES); return mScaleAdj[index];}

    // 时间百分比[0.0~1.0]
	void SetTimeAdjust(size_t index, Real time){ASSERT(index < MAX_STAGES); mTimeAdj[index] = time;}
	Real GetTimeAdjust(size_t index) const{ASSERT(index < MAX_STAGES); return mTimeAdj[index];}

    // 生命周期分成多少个循环(>=1)
    void SetCycles(int nCycles) {m_nCycles = nCycles;}
    int GetCycles() const {return m_nCycles;}
private:
    int m_nCycles;
	Real mTimeAdj[MAX_STAGES];
	Vector2 mScaleAdj[MAX_STAGES];
};

 RefPtr<CParticleScaleInterpolatorAffector> CreateScaleInterpolatorAffector();
}