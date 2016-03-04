#pragma once
#include "ParticleAffector.h"

namespace ma
{

class  ParticleColourInterpolatorAffector : public ParticleAffector
{
public:
	#define MAX_STAGES 6 

	ParticleColourInterpolatorAffector(void);
	~ParticleColourInterpolatorAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from ParticleAffector
	// ---------------------------------------------------------------------
public:
	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:
	void SetColourAdjust(size_t index, const ColourValue& colour){ASSERT(index < MAX_STAGES); mColourAdj[index] = colour;}
	const ColourValue& GetColourAdjust(size_t index) const{ASSERT(index < MAX_STAGES); return mColourAdj[index];}

    // 时间百分比[0.0~1.0]
	void SetTimeAdjust(size_t index, Real time){ASSERT(index < MAX_STAGES); mTimeAdj[index] = time;}
	Real GetTimeAdjust(size_t index) const{ASSERT(index < MAX_STAGES); return mTimeAdj[index];}

    // 生命周期分成多少个循环(>=1)
    void SetCycles(int nCycles) {m_nCycles = nCycles;}
    int GetCycles() const {return m_nCycles;}
private:
    int m_nCycles;
	ColourValue				mColourAdj[MAX_STAGES];
	Real					mTimeAdj[MAX_STAGES];
};

 RefPtr<ParticleColourInterpolatorAffector> CreateColourInterpolatorAffector();
}