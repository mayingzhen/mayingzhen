#ifndef _Particle_H_
#define _Particle_H_


namespace ma
{
	struct Particle
	{
		Vector3 _position;
		Vector3 _velocity;
		Vector3 _acceleration;
		ColourValue _colorStart;
		ColourValue _colorEnd;
		ColourValue _color;
		float _rotationPerParticleSpeed;
		Vector3 _rotationAxis;
		float _rotationSpeed;
		float _angle;
		long _energyStart;
		long _energy;
		float _sizeStart;
		float _sizeEnd;
		float _size;
		UINT _frame;
		float _timeOnCurrentFrame;
		bool _visible;
	};

	struct SpriteTextureInfo
	{
		std::string		m_sTexturePath;
		BLEND_MODE		m_eTextureBlending;

		int				m_nSpriteWidth;
		int				m_nSpriteHeight;
		bool			m_bSpriteAnimated;
		bool			m_bSpriteLooped;
		int				m_nSpriteFrameCount;
		int				m_nSpriteFrameRandomOffset;
		float			m_fSpriteFrameDuration ;
	};

	struct ParticleEmitInfo
	{
		UINT			m_nParticleCountMax;
		UINT			m_nEmissionRate;
		bool			m_bEllipsoid;
		float			m_fSizeStartMin;
		float			m_fSizeStartMax;
		float			m_fSizeEndMin;
		float			m_fSizeEndMax; 
		long			m_nEnergyMin;
		long			m_nEnergyMax; 

		ColourValue		m_colorStart;
		ColourValue		m_colorStartVar;
		ColourValue		m_colorEnd;
		ColourValue		m_colorEndVar;

		Vector3			m_position;
		Vector3			m_positionVar;
		Vector3			m_velocity;
		Vector3			m_velocityVar;
		Vector3			m_acceleration;
		Vector3			m_accelerationVar;
		Vector3			m_rotationAxis;
		Vector3			m_rotationAxisVar;
		float			m_fRotationPerParticleSpeedMin;
		float			m_fRotationPerParticleSpeedMax; 
		float			m_fRotationSpeedMin; 
		float			m_fRotationSpeedMax;
		bool			m_bOrbitPosition; 
		bool			m_bOrbitVelocity; 
		bool			m_bOrbitAcceleration; 

	};


}





#endif