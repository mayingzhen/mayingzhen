#pragma once
#include "ParticleAffector.h"

namespace ma
{

class  CParticleColourFaderAffector : public CParticleAffector
{
public:
	CParticleColourFaderAffector();
	~CParticleColourFaderAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleAffector
	// ---------------------------------------------------------------------
public:
	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	//! Get emitter type
	virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_COLOUR_FADER; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:

	/** Sets the colour adjustment to be made per second to particles. 
    @param red, green, blue, alpha
        Sets the adjustment to be made to each of the colour components per second. These
        values will be added to the colour of all particles every second, scaled over each frame
        for a smooth adjustment.
    */
    // 每秒相加的颜色速度(RGBA)
	void SetColourAdder(const Vector4& adder){mColourAdder = adder;}
	const Vector4& GetColourAdder() const{return mColourAdder;}
private:
	Vector4 mColourAdder;
};

 RefPtr<CParticleColourFaderAffector> CreateColourFaderAffector();

}