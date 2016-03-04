#pragma once
#include "ParticleAffector.h"

namespace ma
{

//! Particle Affector for fading out a color
class  ParticleColourAffector : public ParticleAffector
{
public:
	ParticleColourAffector();
	~ParticleColourAffector(void);

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
    // 渐变到最终的颜色(RGBA)"
	//! Sets the targetColor, i.e. the color the particles will interpolate to over time.
	virtual void SetTargetColor( const ColourValue& targetColor ){mTargetColor = targetColor;}

    // 渐变的时间（秒）
	//! Sets the amount of time it takes for each particle to fade out.
	virtual void SetFadeOutTime( float fadeOutTime ){mFadeOutTime = fadeOutTime;}

	//! Gets the targetColor, i.e. the color the particles will interpolate to over time.
	virtual const ColourValue& GetTargetColor() const{return mTargetColor;}

	//! Gets the amount of time it takes for each particle to fade out.
	virtual float GetFadeOutTime() const{return mFadeOutTime;}

private:
	ColourValue mTargetColor;
	float mFadeOutTime;
};

//! Creates a fade out particle affector.
/** This affector modifies the color of every particle and and reaches
the final color when the particle dies. This affector looks really
good, if the EMT_TRANSPARENT_VERTEX_ALPHA material is used and the
targetColor is video::SColor(0,0,0,0): Particles are fading out into
void with this setting.
\param targetColor: Color whereto the color of the particle is changed.
\param timeNeededToFadeOut: How much time in seconds should the
affector need to change the color to the targetColor.
\return Pointer to the created particle affector. To add this affector
as new affector of this particle system, just call addAffector(). Note
that you'll have to drop() the returned pointer, after you don't need
it any more, see IReferenceCounted::drop() for more informations. */
 RefPtr<ParticleColourAffector> CreateColourAffector();

}