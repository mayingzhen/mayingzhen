#pragma once
#include "ParticleAffector.h"

namespace ma
{

class  CParticleColourFader2Affector : public CParticleAffector
{
public:
	CParticleColourFader2Affector(void);
	~CParticleColourFader2Affector(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleAffector
	// ---------------------------------------------------------------------
public:
	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	//! Get emitter type
	virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_COLOUR_FADER2; }

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
    // 第0级每秒渐变速度（RGBA）
	void SetColourAdder0(const Vector4& adder){mColourAdder0 = adder;}
	const Vector4& GetColourAdder0() const{return mColourAdder0;}

    // 第1级每秒渐变速度（RGBA）
	void SetColourAdder1(const Vector4& adder){mColourAdder1 = adder;}
	const Vector4& GetColourAdder1() const{return mColourAdder1;}

    // 当小于改时刻，渐变速度为ColourAdd0；反之，渐变速度为ColourAdd1
	void SetTimeChange(Real NewValue ){mTimeChange = NewValue;}
	Real GetTimeChange(void) const{return mTimeChange;}
private:
	Vector4 mColourAdder0, mColourAdder1;
	Real mTimeChange;
};

 RefPtr<CParticleColourFader2Affector> CreateColourFader2Affector();

}