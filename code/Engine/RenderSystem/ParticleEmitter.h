#ifndef PARTICLEEMITTER_H_
#define PARTICLEEMITTER_H_

#include "Engine/RenderSystem/RenderObject.h"

namespace ma
{

class Camera;
class SpriteBatch;

/**
 * Defines a particle emitter that can be made to simulate and render a particle system.
 * A ParticleEmitter has a texture and a maximum number of particles that can be alive at
 * once, both of which are set when the ParticleEmitter is created and cannot be changed
 * from then on.  Particles are rendered as camera-facing billboards using the emitter's
 * texture.  The ParticleEmitter's texture properties determine whether the texture
 * is treated as a single image, a texture atlas or an animated sprite.
 *
 * A ParticleEmitter also has a number of properties that determine values assigned to
 * individual particles it emits.  Scalar properties such as particle begin- and end-size
 * are assigned within a minimum and maximum value; vector properties are assigned within
 * the domain defined by a base vector and a variance vector as follows: The variance vector
 * is multiplied by a random scalar between 1 and -1, and the base vector is added to this
 * result.  This allows a ParticleEmitter to be created which emits particles with properties
 * that are randomized, yet fit within a well-defined range.  To make a property deterministic,
 * simply set the minimum to the same value as the maximum for that property or set its
 * variance to the zero vector.
 *
 * <h2>Scalar properties:</h2>
 *
 * Begin-Size: \n
 * The size of a newly emitted particle.
 *
 * End-Size: \n
 * The size of a particle at the end of its lifetime.  A particle's size will
 * interpolate linearly between its begin-size and end-size over its lifetime.
 *
 * Energy: \n
 * The length of time a particle will remain alive for.
 *
 * RotationSpeedPerParticle: \n
 * The speed and direction a particle will spin.  Since particles are
 * rendered as billboards, no axis of rotation can be specified per particle.
 * Each particles rotates around their center points, around the z-axis in
 * screen space.
 *
 * RotationSpeed:\n
 * The speed a particle will spin around its RotationAxis in world space.
 * (See RotationAxis under "Vector properties" below.)
 *
 *
 * <h2>Vector properties:</h2>
 * 
 * Initial Position: \n
 * The position of a new particle at the moment it is emitted, relative
 * to the node its ParticleEmitter is set on.  This property is unique
 * in that the initial positions of new particles can be restricted to
 * fit within an ellipsoidal domain; see setEllipsoid().
 *
 * Initial Velocity: \n
 * The velocity of a new particle at the moment it is emitted.  This
 * property is measured in world coordinates per second and modifies
 * a particle's current position each time ParticleEmitter::update()
 * is called.
 *
 * Acceleration:\n
 * The particle's change in velocity, measured in world coordinates per second.
 * This property modifies a particle's current position each time 
 * ParticleEmitter::update() is called.
 *
 * Color: \n
 * The color of a particle at the end of its lifetime.  A particle's color
 * will interpolate linearly between its begin-color and end-color over its lifetime.
 *
 * RotationAxis: \n
 * An axis in world space around which all particles will spin, allowing for tornado and
 * spiral effects.
 *
 * The vector properties Initial Position, Initial Velocity and Acceleration can be set to
 * orbit around the origin of a node a ParticleEmitter is set on by that node's rotation matrix.
 * This allows the rotation of a node, and not just its position, to affect these properties of
 * newly emitted particles.  An example of where this would be useful would be a water-fountain
 * emitter attached to the nozzle of a hose.  The initial position and initial velocity would be
 * set to orbit around the node's origin so that the water would always spray out in the direction
 * the nozzle was facing.  However, acceleration would not be set to orbit the node's origin in
 * order for gravity to continue to act in the same direction on water particles, no matter
 * what direction they were originally aimed.
 * 
 * <h2>Rendering properties:</h2>
 *
 * Particles are rendered as screen-facing billboards -- that is, the ParticleEmitter's texture
 * is used to render particles as images that always face the camera.  For the simplest case,
 * where the entire texture is used for every particle, the default texture settings can be used.
 * However, a ParticleEmitter can also be configured to select one of several frames at random
 * for each particle, or to render each particle as a sprite that animates through the frames
 * over the course of its lifetime.
 *
 * Frame Count: \n
 * The number of individual images / frames contained in the texture.
 *
 * Texture Coordinates: \n
 * The coordinates within the texture used to render a specific frame.
 * Using a texture that places the frames together, without padding,
 * in left-to-right top-to-bottom order is recommended, as there is a utility
 * method for generating the texture coordinates for such a texture atlas /
 * sprite-map.  See setSpriteFrameCoords().
 *
 * Sprite Animating: \n
 * Set this to enable sprite animation.
 *
 * Sprite Looped: \n
 * If sprites are set to loop, each frame will last for the emitter's frameDuration.
 * If sprites are set not to loop, the animation will be timed so that the last frame
 * finishes just as a particle dies.  This setting has no effect if the sprite is not
 * animating.
 *
 * Sprite Random Offset: \n
 * New particles are created with one of the sprite frames in the emitter's texture.
 * If a maximum offset is set, a random frame from 0 to maxOffset will be selected.
 * If sprite animation is disabled and this offset is set to Frame Count, each
 * particle will use one of the sprite frames for its entire lifetime.
 *
 * Blend Mode: \n
 * Sets the blend mode used by this particle emitter.  The given blend factors will
 * be set before rendering the particle system and then will be reset to their original
 * values.  Accepts the same symbolic constants as glBlendFunc().
 *
 */
class ENGINE_API ParticleEmitter : public RenderObject
{
 
public:

    static ParticleEmitter* create(const char* url);

    static ParticleEmitter* create(Properties* properties);

    static ParticleEmitter* create(const char* texturePath, BLEND_MODE textureBlending,  unsigned int particleCountMax);

	Renderable*	GetRenderable();

    void setEmissionRate(unsigned int rate);

    unsigned int getEmissionRate() const;

    void start();

    void stop();

    bool isStarted() const;

    bool isActive() const;

    void emitOnce(unsigned int particleCount);

    unsigned int getParticlesCount() const;

    void setEllipsoid(bool ellipsoid);

    bool isEllipsoid() const;

    void setSize(float startMin, float startMax, float endMin, float endMax);

    float getSizeStartMin() const;

    float getSizeStartMax() const;

    float getSizeEndMin() const;

    float getSizeEndMax() const;

    void setColor(const Vector4& start, const Vector4& startVariance, const Vector4& end, const Vector4& endVariance);
    
    const Vector4& getColorStart() const;

    const Vector4& getColorStartVariance() const;

    const Vector4& getColorEnd() const;

    const Vector4& getColorEndVariance() const;

    void setEnergy(long energyMin, long energyMax);

    long getEnergyMin() const;

    long getEnergyMax() const;

    void setPosition(const Vector3& position, const Vector3& positionVariance);

    const Vector3& getPosition() const;

    const Vector3& getPositionVariance() const;

    void setVelocity(const Vector3& velocity, const Vector3& velocityVariance);

    const Vector3& getVelocity() const;

    const Vector3& getVelocityVariance() const;

    const Vector3& getAcceleration() const;

    void setAcceleration(const Vector3& acceleration, const Vector3& accelerationVariance);

    const Vector3& getAccelerationVariance() const;

    void setRotationPerParticle(float speedMin, float speedMax);

    float getRotationPerParticleSpeedMin() const;

    float getRotationPerParticleSpeedMax() const;

	void setRotation(float speedMin, float speedMax, const Vector3& axis, const Vector3& axisVariance);

    float getRotationSpeedMin() const;

    float getRotationSpeedMax() const;

    const Vector3& getRotationAxis() const;

    const Vector3& getRotationAxisVariance() const;

    void setSpriteAnimated(bool animated);

    bool isSpriteAnimated() const;

    void setSpriteLooped(bool looped);

    bool isSpriteLooped() const;

    void setSpriteFrameRandomOffset(int maxOffset);
    
    int getSpriteFrameRandomOffset() const;

    void setSpriteFrameDuration(long duration);

    long getSpriteFrameDuration() const;

    void setSpriteTexCoords(unsigned int frameCount, float* texCoords);

    void setSpriteFrameCoords(unsigned int frameCount, Rectangle* frameCoords);

    void setSpriteFrameCoords(unsigned int frameCount, int width, int height);

    void setOrbit(bool orbitPosition, bool orbitVelocity, bool orbitAcceleration);

    void Update();

    virtual	void		Render();

	virtual Material*	GetMaterial();

	void SetWorldMatrix(const Matrix4x4& matWorld);

    static BLEND_MODE getTextureBlendingFromString(const char* src);

    void setTextureBlending(BLEND_MODE blending);

	void	SetUpdate(bool bUpadate) {m_bUpdate = bUpadate;}

private:

    ParticleEmitter(SpriteBatch* batch, unsigned int particlesCount);

    ~ParticleEmitter();

    ParticleEmitter& operator=(const ParticleEmitter&);

    // Generates a scalar within the range defined by min and max.
    float generateScalar(float min, float max);

    long generateScalar(long min, long max);

    // Generates a vector within the domain defined by a base vector and its variance.
    void generateVectorInRect(const Vector3& base, const Vector3& variance, Vector3* dst);

    // Generates a vector within the ellipsoidal domain defined by a center point and scale vector.
    void generateVectorInEllipsoid(const Vector3& center, const Vector3& scale, Vector3* dst);

    void generateVector(const Vector3& base, const Vector3& variance, Vector3* dst, bool ellipsoid);

    // Generates a color within the domain defined by a base vector and its variance.
    void generateColor(const Vector4& base, const Vector4& variance, Vector4* dst);

	void	UpdateParticles();

	void	UpdateMeshBatch();

    /**
     * Defines the data for a single particle in the system.
     */
    class Particle
    {

    public:
        Vector3 _position;
        Vector3 _velocity;
        Vector3 _acceleration;
        Vector4 _colorStart;
        Vector4 _colorEnd;
        Vector4 _color;
        float _rotationPerParticleSpeed;
        Vector3 _rotationAxis;
        float _rotationSpeed;
        float _angle;
        long _energyStart;
        long _energy;
        float _sizeStart;
        float _sizeEnd;
        float _size;
        unsigned int _frame;
        float _timeOnCurrentFrame;
        bool _visible;
    };

    unsigned int _particleCountMax;
    unsigned int _particleCount;
    Particle* _particles;
    unsigned int _emissionRate;
    bool _started;
    bool _ellipsoid;
    float _sizeStartMin;
    float _sizeStartMax;
    float _sizeEndMin;
    float _sizeEndMax;
    float _energyMin;
    float _energyMax;
    Vector4 _colorStart;
    Vector4 _colorStartVar;
    Vector4 _colorEnd;
    Vector4 _colorEndVar;
    Vector3 _position;
    Vector3 _positionVar;
    Vector3 _velocity;
    Vector3 _velocityVar;
    Vector3 _acceleration;
    Vector3 _accelerationVar;
    float _rotationPerParticleSpeedMin;
    float _rotationPerParticleSpeedMax;
    float _rotationSpeedMin;
    float _rotationSpeedMax;
    Vector3 _rotationAxis;
    Vector3 _rotationAxisVar;
    Matrix4x4 _rotation;
    SpriteBatch* _spriteBatch;
    BLEND_MODE _spriteTextureBlending;
    float _spriteTextureWidth;
    float _spriteTextureHeight;
    float _spriteTextureWidthRatio;
    float _spriteTextureHeightRatio;
    float* _spriteTextureCoords;
    bool _spriteAnimated;
    bool _spriteLooped;
    unsigned int _spriteFrameCount;
    unsigned int _spriteFrameRandomOffset;
    long _spriteFrameDuration;
    float _spriteFrameDurationSecs;
    float _spritePercentPerFrame;
    bool _orbitPosition;
    bool _orbitVelocity;
    bool _orbitAcceleration;
    float _timePerEmission;
    double _timeRunning;

	CriticalSection		m_csUpdate;	// 

	volatile bool		m_bUpdate;
};

}

#endif
