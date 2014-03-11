#ifndef PARTICLEEMITTER_H_
#define PARTICLEEMITTER_H_


namespace ma
{

	class Camera;
	class SpriteBatch;

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
		UINT		m_nParticleCountMax;
		UINT		m_nEmissionRate;
		bool		m_bEllipsoid;
		float		m_fSizeStartMin;
		float		m_fSizeStartMax;
		float		m_fSizeEndMin;
		float		m_fSizeEndMax; 
		long		m_nEnergyMin;
		long		m_nEnergyMax; 

		Vector4		m_colorStart;
		Vector4		m_colorStartVar;
		Vector4		m_colorEnd;
		Vector4		m_colorEndVar;

		Vector3		m_position;
		Vector3		m_positionVar;
		Vector3		m_velocity;
		Vector3		m_velocityVar;
		Vector3		m_acceleration;
		Vector3		m_accelerationVar;
		Vector3		m_rotationAxis;
		Vector3		m_rotationAxisVar;
		float		m_fRotationPerParticleSpeedMin;
		float		m_fRotationPerParticleSpeedMax; 
		float		m_fRotationSpeedMin; 
		float		m_fRotationSpeedMax;
		bool		m_bOrbitPosition; 
		bool		m_bOrbitVelocity; 
		bool		m_bOrbitAcceleration; 

	};

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


	class RENDER_API ParticleEmitter : public RenderComponent
	{
		DECL_OBJECT(ParticleEmitter)

	public:
		ParticleEmitter(GameObject* pGameObj);

		~ParticleEmitter();

		void				Load(const char* pParticleEmitPath);
	
		Renderable*			GetRenderable();

		void				start();

		void				stop();

		bool				isStarted() const;

		bool				isActive() const;

		void				Update();

		virtual	void		Render(Technique* pTech);

		virtual Material*	GetMaterial();

		//virtual void		SetMaterial(Material* pMaterial);

		virtual	void		Show(Camera* pCamera);

		//virtual void		SetWorldMatrix(const Matrix4x4& matWS);

		virtual void		Serialize(Serializer& sl, const char* pszLable = "ParticleEmitter");

		void				SetUpdate(bool bUpadate) {m_bUpdate = bUpadate;}

		static BLEND_MODE	getTextureBlendingFromString(const char* src);

	
	private:

		// Load
		void				LoadSpriteInfo(Properties* pSpriteProp,SpriteTextureInfo& info);

		void				LoadParticleEmitInfo(Properties* pProperties,ParticleEmitInfo& info);

		void				setSpriteFrameCoords(int nTextureWidth, int nTextureHeight);

		//  Update
		void				UpdateParticles();

		void				emitOnce(UINT particleCount);

		// Generates Data
		float				generateScalar(float min, float max);

		long				generateScalar(long min, long max);

		void				generateVectorInRect(const Vector3& base, const Vector3& variance, Vector3* dst);

		void				generateVectorInEllipsoid(const Vector3& center, const Vector3& scale, Vector3* dst);

		void				generateVector(const Vector3& base, const Vector3& variance, Vector3* dst, bool ellipsoid);

		void				generateColor(const Vector4& base, const Vector4& variance, Vector4* dst);

	private:
		UINT				m_nParticleCount;

		Particle*			m_pParticles;

		SpriteBatch*		m_pSpriteBatch;

		Material*			m_pMaterial;

		bool				m_bStarted;
	 
		float*				m_pSpriteTextureCoords;

		SpriteTextureInfo	m_spriteTextureInfo;

		ParticleEmitInfo	m_particleEmitInfo;

		float				m_fTimeRunning;


		CriticalSection		m_csUpdate;	// 

		volatile bool		m_bUpdate;
	};

}

#endif
