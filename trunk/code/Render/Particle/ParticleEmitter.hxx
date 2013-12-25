#include "ParticleEmitter.h"

#define PARTICLE_COUNT_MAX                       100
#define PARTICLE_EMISSION_RATE                   10
#define PARTICLE_EMISSION_RATE_TIME_INTERVAL     1000.0f / (float)PARTICLE_EMISSION_RATE

namespace ma
{

	IMPL_OBJECT(ParticleEmitter,RenderObject)

	ParticleEmitter::ParticleEmitter(GameObject* pGameObj)
		:RenderObject(pGameObj)
	{
		m_pSpriteBatch = NULL;
		m_pParticles = NULL;
		m_pSpriteTextureCoords = NULL;
		m_nParticleCount = 0;
		m_bStarted = false;
		m_fTimeRunning = 0;
		m_bUpdate = false;
	}

	ParticleEmitter::~ParticleEmitter()
	{
		SAFE_DELETE(m_pSpriteBatch);
		SAFE_DELETE_ARRAY(m_pParticles);
		SAFE_DELETE_ARRAY(m_pSpriteTextureCoords);
	}


	void ParticleEmitter::LoadSpriteInfo(Properties* pSpriteProp,SpriteTextureInfo& info)
	{
		if (!pSpriteProp->getPath("path", &info.m_sTexturePath))
		{
			GP_ERROR("Failed to load particle emitter: required image file path ('path') is missing.");
			return ;
		}

		const char* blendingString = pSpriteProp->getString("blending");
		info.m_eTextureBlending = getTextureBlendingFromString(blendingString);
		info.m_nSpriteWidth = pSpriteProp->getInt("width");
		info.m_nSpriteHeight = pSpriteProp->getInt("height");
		info.m_bSpriteAnimated = pSpriteProp->getBool("animated");
		info.m_bSpriteLooped = pSpriteProp->getBool("looped");
		info.m_nSpriteFrameCount = pSpriteProp->getInt("frameCount");
		info.m_nSpriteFrameRandomOffset = pSpriteProp->getInt("frameRandomOffset");
		info.m_fSpriteFrameDuration = pSpriteProp->getFloat("frameDuration");

		Texture* pTexture = LoadResourceSync<Texture>(info.m_sTexturePath.c_str());
		ASSERT(pTexture);
		if (pTexture == NULL)
			return;

		setSpriteFrameCoords( pTexture->getWidth(), pTexture->getHeight());

		SAFE_DELETE(m_pSpriteBatch);
		m_pSpriteBatch = new SpriteBatch(pTexture);

		m_pSpriteBatch->getStateBlock().m_eBlendMode = info.m_eTextureBlending;


		m_pMaterial = new Material();
		m_pMaterial->AddTechnique(m_pSpriteBatch->GetTechnique());
	}


	void ParticleEmitter::LoadParticleEmitInfo(Properties* pProperties,ParticleEmitInfo& info)
	{
		// Emitter properties.
		info.m_nParticleCountMax = (UINT)pProperties->getInt("particleCountMax");
		if (info.m_nParticleCountMax == 0)
		{
			info.m_nParticleCountMax = PARTICLE_COUNT_MAX;
		}

		info.m_nEmissionRate = (UINT)pProperties->getInt("emissionRate");
		if (info.m_nEmissionRate == 0)
		{
			info.m_nEmissionRate = PARTICLE_EMISSION_RATE;
		}

		info.m_bEllipsoid = pProperties->getBool("ellipsoid");
		info.m_fSizeStartMin = pProperties->getFloat("sizeStartMin");
		info.m_fSizeStartMax = pProperties->getFloat("sizeStartMax");
		info.m_fSizeEndMin = pProperties->getFloat("sizeEndMin");
		info.m_fSizeEndMax = pProperties->getFloat("sizeEndMax");
		info.m_nEnergyMin = pProperties->getLong("energyMin");
		info.m_nEnergyMax = pProperties->getLong("energyMax");

		// color
		pProperties->getVector4("colorStart", &info.m_colorStart);
		pProperties->getVector4("colorStartVar", &info.m_colorStartVar);
		pProperties->getVector4("colorEnd", &info.m_colorEnd);
		pProperties->getVector4("colorEndVar", &info.m_colorEndVar);
	
		// pos
		pProperties->getVector3("position", &info.m_position);
		pProperties->getVector3("positionVar", &info.m_positionVar);
		pProperties->getVector3("velocity", &info.m_velocity);
		pProperties->getVector3("velocityVar", &info.m_velocityVar);
		pProperties->getVector3("acceleration", &info.m_acceleration);
		pProperties->getVector3("accelerationVar", &info.m_accelerationVar);
		info.m_fRotationPerParticleSpeedMin = pProperties->getFloat("rotationPerParticleSpeedMin");
		info.m_fRotationPerParticleSpeedMax = pProperties->getFloat("rotationPerParticleSpeedMax");
		info.m_fRotationSpeedMin = pProperties->getFloat("rotationSpeedMin");
		info.m_fRotationSpeedMax = pProperties->getFloat("rotationSpeedMax");
		pProperties->getVector3("rotationAxis", &info.m_rotationAxis);
		pProperties->getVector3("rotationAxisVar", &info.m_rotationAxisVar);
		info.m_bOrbitPosition = pProperties->getBool("orbitPosition");
		info.m_bOrbitVelocity = pProperties->getBool("orbitVelocity");
		info.m_bOrbitAcceleration = pProperties->getBool("orbitAcceleration");


		/// 
		m_pParticles = new Particle[info.m_nParticleCountMax];
	}

	void ParticleEmitter::Load(const char* pParticleEmitPath)
	{
		Properties* pProperties = Properties::create(pParticleEmitPath);
		ASSERT(pProperties);
		if (pProperties == NULL)
			return;

		pProperties = strlen(pProperties->getNamespace()) > 0 ? pProperties : pProperties->getNextNamespace();
		if (!pProperties || strcmp(pProperties->getNamespace(), "particle") != 0)
		{
			ASSERT(false && "Properties object must be non-null and have namespace equal to 'particle'.");
			return ;
		}

		Properties* pSpriteProp = pProperties->getNextNamespace();
		if (!pSpriteProp || strcmp(pSpriteProp->getNamespace(), "sprite") != 0)
		{
			ASSERT(false && "Failed to load particle emitter: required namespace 'sprite' is missing.");
			return;
		}

		LoadSpriteInfo(pSpriteProp,m_spriteTextureInfo);

		LoadParticleEmitInfo(pProperties,m_particleEmitInfo);

		SAFE_DELETE(pProperties);
	}


	Renderable*	ParticleEmitter::GetRenderable() 
	{
		return m_pSpriteBatch->GetMeshBatch();
	}

	void ParticleEmitter::start()
	{
		m_bStarted = true;
	}

	void ParticleEmitter::stop()
	{
		m_bStarted = false;
	}

	bool ParticleEmitter::isStarted() const
	{
		return m_bStarted;
	}

	bool ParticleEmitter::isActive() const
	{
		if (m_bStarted)
			return true;

		ASSERT(m_pParticles);
		bool active = false;
		for (unsigned int i = 0; i < m_nParticleCount; i++)
		{
			if (m_pParticles[i]._energy > 0)
			{
				active = true;
				break;
			}
		}

		return active;
	}

	void ParticleEmitter::emitOnce(UINT particleCount)
	{
		// Limit particleCount so as not to go over _particleCountMax.
		if (particleCount + m_nParticleCount > m_particleEmitInfo.m_nParticleCountMax)
		{
			particleCount = m_particleEmitInfo.m_nParticleCountMax - m_nParticleCount;
		}

		// Emit the new particles.
		for (unsigned int i = 0; i < particleCount; i++)
		{
			Particle* p = &m_pParticles[m_nParticleCount];
			p->_visible = true;

			generateColor(m_particleEmitInfo.m_colorStart, m_particleEmitInfo.m_colorStartVar, &p->_colorStart);
			generateColor(m_particleEmitInfo.m_colorEnd, m_particleEmitInfo.m_colorEndVar, &p->_colorEnd);
			p->_color = p->_colorStart;

			p->_energy = p->_energyStart = generateScalar(m_particleEmitInfo.m_nEnergyMin, m_particleEmitInfo.m_nEnergyMax);
			p->_size = p->_sizeStart = generateScalar(m_particleEmitInfo.m_fSizeStartMin, m_particleEmitInfo.m_fSizeStartMax);
			p->_sizeEnd = generateScalar(m_particleEmitInfo.m_fSizeEndMin, m_particleEmitInfo.m_fSizeEndMax);
			p->_rotationPerParticleSpeed = generateScalar(m_particleEmitInfo.m_fRotationPerParticleSpeedMin, m_particleEmitInfo.m_fRotationPerParticleSpeedMax);
			p->_angle = generateScalar(0.0f, p->_rotationPerParticleSpeed);
			p->_rotationSpeed = generateScalar(m_particleEmitInfo.m_fRotationSpeedMin, m_particleEmitInfo.m_fRotationSpeedMax);

			// Only initial position can be generated within an ellipsoidal domain.
			generateVector(m_particleEmitInfo.m_position, m_particleEmitInfo.m_positionVar, &p->_position, m_particleEmitInfo.m_bEllipsoid);
			generateVector(m_particleEmitInfo.m_velocity, m_particleEmitInfo.m_velocityVar, &p->_velocity, false);
			generateVector(m_particleEmitInfo.m_acceleration, m_particleEmitInfo.m_accelerationVar, &p->_acceleration, false);
			generateVector(m_particleEmitInfo.m_rotationAxis, m_particleEmitInfo.m_rotationAxisVar, &p->_rotationAxis, false);

			// Initial position, velocity and acceleration can all be relative to the emitter's transform.
			// Rotate specified properties by the node's rotation.
			if (m_particleEmitInfo.m_bOrbitPosition)
			{
				//Vec3TransformCoord(&p->_position,&p->_position,&world);
				//world.transformPoint(p->_position, &p->_position);
			}

			if (m_particleEmitInfo.m_bOrbitVelocity)
			{
				//Vec3TransformCoord(&p->_velocity,&p->_velocity,&world);
				//world.transformPoint(p->_velocity, &p->_velocity);
			}

			if (m_particleEmitInfo.m_bOrbitAcceleration)
			{
				//Vec3TransformCoord(&p->_acceleration,&p->_acceleration,&world);
				//world.transformPoint(p->_acceleration, &p->_acceleration);
			}

			// The rotation axis always orbits the node.
			if (p->_rotationSpeed != 0.0f && p->_rotationAxis != Vec3Zero() )
			{
				//Vec3TransformCoord(&p->_rotationAxis,&p->_rotationAxis,&world);
				//world.transformPoint(p->_rotationAxis, &p->_rotationAxis);
			}

			// Translate position relative to the node's world space.
			//p->_position += translation;
			//p->_position.add(translation);

			// Initial sprite frame.
			if (m_spriteTextureInfo.m_nSpriteFrameRandomOffset > 0)
			{
				p->_frame = rand() % m_spriteTextureInfo.m_nSpriteFrameRandomOffset;
			}
			else
			{
				p->_frame = 0;
			}
			p->_timeOnCurrentFrame = 0.0f;

			++m_nParticleCount;
		}
	}


	void ParticleEmitter::setSpriteFrameCoords(int nTextureWidth, int nTextureHeight)
	{
		ASSERT(nTextureWidth);
		ASSERT(nTextureHeight);

		int nFrameCount = m_spriteTextureInfo.m_nSpriteFrameCount;
		int nSpriteWidth = m_spriteTextureInfo.m_nSpriteWidth;
		int nSpriteHeight = m_spriteTextureInfo.m_nSpriteHeight;

		int x;
		int y;
		Rectangle* frameCoords = new Rectangle[nFrameCount];
		int cols = nTextureWidth / nSpriteWidth;
		int rows = nTextureHeight / nSpriteHeight;

		unsigned int n = 0;
		for (int i = 0; i < rows; ++i)
		{
			y = i * nSpriteHeight;
			for (int j = 0; j < cols; ++j)
			{
				x = j * nSpriteWidth;
				frameCoords[i*cols + j] = Rectangle((float)x, (float)y, (float)nSpriteWidth, (float)nSpriteHeight);
				if (++n == nFrameCount)
				{
					break;
				}
			}

			if (n == nFrameCount)
			{
				break;
			}
		}

		//float fSpritePercentPerFrame = 1.0f / (float)nFrameCount;

		SAFE_DELETE_ARRAY(m_pSpriteTextureCoords);
		m_pSpriteTextureCoords = new float[nFrameCount * 4];

		// Pre-compute texture coordinates from rects.
		for (int i = 0; i < nFrameCount; i++)
		{
			m_pSpriteTextureCoords[i*4] = frameCoords[i].x / (float)nTextureWidth;
			m_pSpriteTextureCoords[i*4 + 1] = frameCoords[i].y / (float)nTextureHeight;
			m_pSpriteTextureCoords[i*4 + 2] = m_pSpriteTextureCoords[i*4] + frameCoords[i].width / (float)nTextureWidth;
			m_pSpriteTextureCoords[i*4 + 3] = m_pSpriteTextureCoords[i*4 + 1] + frameCoords[i].height / (float)nTextureHeight;
		}

		SAFE_DELETE_ARRAY(frameCoords);
	}



	long ParticleEmitter::generateScalar(long min, long max)
	{
		// Note: this is not a very good RNG, but it should be suitable for our purposes.
		long r = 0;
		for (unsigned int i = 0; i < sizeof(long)/sizeof(int); i++)
		{
			r = r << 8; // sizeof(int) * CHAR_BITS
			r |= rand();
		}

		// Now we have a random long between 0 and MAX_LONG.  We need to clamp it between min and max.
		r %= max - min;
		r += min;

		return r;
	}

	float ParticleEmitter::generateScalar(float min, float max)
	{
		return min + (max - min) * MATH_RANDOM_0_1();
	}

	void ParticleEmitter::generateVectorInRect(const Vector3& base, const Vector3& variance, Vector3* dst)
	{
		ASSERT(dst);

		// Scale each component of the variance vector by a random float
		// between -1 and 1, then add this to the corresponding base component.
		dst->x = base.x + variance.x * MATH_RANDOM_MINUS1_1();
		dst->y = base.y + variance.y * MATH_RANDOM_MINUS1_1();
		dst->z = base.z + variance.z * MATH_RANDOM_MINUS1_1();
	}

	void ParticleEmitter::generateVectorInEllipsoid(const Vector3& center, const Vector3& scale, Vector3* dst)
	{
		ASSERT(dst);

		// Generate a point within a unit cube, then reject if the point is not in a unit sphere.
		do
		{
			dst->x = MATH_RANDOM_MINUS1_1();
			dst->y = MATH_RANDOM_MINUS1_1();
			dst->z = MATH_RANDOM_MINUS1_1();
		} while ( Vec3Length(dst) > 1.0f);
	    
		// Scale this point by the scaling vector.
		dst->x *= scale.x;
		dst->y *= scale.y;
		dst->z *= scale.z;

		// Translate by the center point.
		*dst += center;
	}

	void ParticleEmitter::generateVector(const Vector3& base, const Vector3& variance, Vector3* dst, bool ellipsoid)
	{
		if (ellipsoid)
		{
			generateVectorInEllipsoid(base, variance, dst);
		}
		else
		{
			generateVectorInRect(base, variance, dst);
		}
	}

	void ParticleEmitter::generateColor(const Vector4& base, const Vector4& variance, Vector4* dst)
	{
		ASSERT(dst);

		// Scale each component of the variance color by a random float
		// between -1 and 1, then add this to the corresponding base component.
		dst->x = base.x + variance.x * MATH_RANDOM_MINUS1_1();
		dst->y = base.y + variance.y * MATH_RANDOM_MINUS1_1();
		dst->z = base.z + variance.z * MATH_RANDOM_MINUS1_1();
		dst->w = base.w + variance.w * MATH_RANDOM_MINUS1_1();
	}

	BLEND_MODE ParticleEmitter::getTextureBlendingFromString(const char* str)
	{
		ASSERT(str);

		if (strcmp(str, "BLEND_OPAQUE") == 0 || strcmp(str, "OPAQUE") == 0)
		{
			return BM_OPATICY;
		}
		else if (strcmp(str, "BLEND_TRANSPARENT") == 0 || strcmp(str, "TRANSPARENT") == 0)
		{
			return BM_TRANSPARENT;
		}
		else if (strcmp(str, "BLEND_ADDITIVE") == 0 || strcmp(str, "ADDITIVE") == 0)
		{
			return BM_ADD;
		}
		else if (strcmp(str, "BLEND_MULTIPLIED") == 0 || strcmp(str, "MULTIPLIED") == 0)
		{
			return BM_MULTIPLY;
		}
		else
		{
			return BM_TRANSPARENT;
		}
	}

	void ParticleEmitter::UpdateParticles()
	{
		// Calculate the time passed since last update.
		float elapsedSecs = GetTimer()->GetFrameDeltaTime() /** 0.001f*/;

		if (m_bStarted && m_particleEmitInfo.m_nEmissionRate)
		{
			float ftimePerEmission = 1000.0f / m_particleEmitInfo.m_nEmissionRate;

			// Calculate how much time has passed since we last emitted particles.
			m_fTimeRunning += elapsedSecs * 1000;

			// How many particles should we emit this frame?
			ASSERT(ftimePerEmission);
			unsigned int emitCount = (unsigned int)(m_fTimeRunning / ftimePerEmission);

			if (emitCount)
			{
				if ((int)ftimePerEmission > 0)
				{
					m_fTimeRunning = fmod(m_fTimeRunning, ftimePerEmission);
				}
				emitOnce(emitCount);
			}
		}

		//ASSERT(_node && _node->getScene() && _node->getScene()->getActiveCamera());
		//const Frustum& frustum = _node->getScene()->getActiveCamera()->getFrustum();

		// Now update all currently living particles.
		ASSERT(m_pParticles);
		for (UINT particlesIndex = 0; particlesIndex < m_nParticleCount; ++particlesIndex)
		{
			Particle* p = &m_pParticles[particlesIndex];
			p->_energy -= (long)(elapsedSecs * 1000);

			if (p->_energy > 0L)
			{
				if (p->_rotationSpeed != 0.0f && p->_rotationAxis != Vec3Zero())
				{
					Matrix4x4 matRotation;

					MatrixRotationAxis(&matRotation,&p->_rotationAxis, p->_rotationSpeed * elapsedSecs);

					Vec3TransformNormal(&p->_velocity,&p->_velocity,&matRotation);
					Vec3TransformNormal(&p->_acceleration,&p->_acceleration,&matRotation);
				}

				// Particle is still alive.
				p->_velocity.x += p->_acceleration.x * elapsedSecs;
				p->_velocity.y += p->_acceleration.y * elapsedSecs;
				p->_velocity.z += p->_acceleration.z * elapsedSecs;

				p->_position.x += p->_velocity.x * elapsedSecs;
				p->_position.y += p->_velocity.y * elapsedSecs;
				p->_position.z += p->_velocity.z * elapsedSecs;

				//if (!frustum.intersects(p->_position))
				//{
				//     p->_visible = false;
				//     continue;
				//}

				p->_angle += p->_rotationPerParticleSpeed * elapsedSecs;

				// Simple linear interpolation of color and size.
				float percent = 1.0f - ((float)p->_energy / (float)p->_energyStart);

				p->_color.x = p->_colorStart.x + (p->_colorEnd.x - p->_colorStart.x) * percent;
				p->_color.y = p->_colorStart.y + (p->_colorEnd.y - p->_colorStart.y) * percent;
				p->_color.z = p->_colorStart.z + (p->_colorEnd.z - p->_colorStart.z) * percent;
				p->_color.w = p->_colorStart.w + (p->_colorEnd.w - p->_colorStart.w) * percent;

				p->_size = p->_sizeStart + (p->_sizeEnd - p->_sizeStart) * percent;

				// Handle sprite animations.
				if (m_spriteTextureInfo.m_bSpriteAnimated)
				{
					if (!m_spriteTextureInfo.m_bSpriteLooped)
					{
						float fSpritePercentPerFrame = 1.0f / (float)m_spriteTextureInfo.m_nSpriteFrameCount;
						// The last frame should finish exactly when the particle dies.
						float percentSpent = 0.0f;
						for (unsigned int i = 0; i < p->_frame; i++)
						{
							percentSpent += fSpritePercentPerFrame;
						}
						p->_timeOnCurrentFrame = percent - percentSpent;
						if (p->_frame < m_spriteTextureInfo.m_nSpriteFrameCount - 1 &&
							p->_timeOnCurrentFrame >= fSpritePercentPerFrame)
						{
							++p->_frame;
						}
					}
					else
					{
						// _spriteFrameDurationSecs is an absolute time measured in seconds,
						// and the animation repeats indefinitely.
						float fSpriteFrameDurationSecs = (float)m_spriteTextureInfo.m_fSpriteFrameDuration / 1000.0f;
						p->_timeOnCurrentFrame += elapsedSecs;
						if (p->_timeOnCurrentFrame >= fSpriteFrameDurationSecs)
						{
							p->_timeOnCurrentFrame -= fSpriteFrameDurationSecs;
							++p->_frame;
							if (p->_frame == m_spriteTextureInfo.m_nSpriteFrameCount)
							{
								p->_frame = 0;
							}
						}
					}
				}
			}
			else
			{
				// Particle is dead.  Move the particle furthest from the start of the array
				// down to take its place, and re-use the slot at the end of the list of living particles.
				if (particlesIndex != m_nParticleCount - 1)
				{
					m_pParticles[particlesIndex] = m_pParticles[m_nParticleCount - 1];
				}
				--m_nParticleCount;
			}
		}

	}

	void ParticleEmitter::Update()
	{
		if (!isActive())
		{
			return;
		}

		AutoLock lock(m_csUpdate);

		if (m_bUpdate)
			return;

		UpdateParticles();
		
		m_bUpdate = true; 
	}


	void ParticleEmitter::Render(Technique* pTech)
	{
		if (!m_bUpdate)
		{
			Update();
		}


		if (m_nParticleCount <= 0)
			return;

		ASSERT(m_pSpriteBatch);
		ASSERT(m_pParticles);
		ASSERT(m_pSpriteTextureCoords);

		// Set our node's view projection matrix to this emitter's effect.
		if (GetRenderSystem())
		{
			int index = GetRenderSystem()->CurThreadProcess();
			Matrix4x4 matWorld = GetRenderable()->m_matWorld[index];
			Matrix4x4 matVP = GetRenderContext()->GetViewProjMatrix();
			m_pSpriteBatch->setProjectionMatrix(matWorld * matVP);
		}

		// Begin sprite batch drawing
		m_pSpriteBatch->start();

		// 2D Rotation.
		static const Vector2 pivot(0.5f, 0.5f);

		// 3D Rotation so that particles always face the camera.
		Matrix4x4 cameraWorldMatrix ;
		MatrixInverse(&cameraWorldMatrix,NULL,&GetRenderContext()->GetViewMatrix());

		Vector3 right;
		//cameraWorldMatrix.getRightVector(&right);
		right = cameraWorldMatrix.GetRow(0);
		Vector3 up;
		//cameraWorldMatrix.getUpVector(&up);
		up = cameraWorldMatrix.GetRow(1);

		for (unsigned int i = 0; i < m_nParticleCount; i++)
		{
			Particle* p = &m_pParticles[i];

			if (p->_visible)
			{
				m_pSpriteBatch->draw(p->_position, right, up, p->_size, p->_size,
					m_pSpriteTextureCoords[p->_frame * 4], 
					m_pSpriteTextureCoords[p->_frame * 4 + 1], 
					m_pSpriteTextureCoords[p->_frame * 4 + 2], 
					m_pSpriteTextureCoords[p->_frame * 4 + 3],
					p->_color, pivot, p->_angle);
			}
		}

		m_pSpriteBatch->finish(pTech);
	}

	Material*	ParticleEmitter::GetMaterial()
	{
		return  m_pMaterial;
	}

	void ParticleEmitter::SetMaterial(Material* pMaterial)
	{

	}

	void ParticleEmitter::AddToRenderQueue() 
	{
		if ( GetParticleSystem() )
			GetParticleSystem()->AddParticleEmitter(this);

		GetRenderSystem()->GetRenderQueue()->AddRenderObj(RL_Trans,this);
	}


	void ParticleEmitter::SetWorldMatrix(const Matrix4x4& matWorld)
	{
		int index = GetRenderSystem()->CurThreadFill();

		Renderable* pRenderable = GetRenderable();
		ASSERT(pRenderable);
		if (pRenderable == NULL)
			return;

		pRenderable->m_matWorld[index] = matWorld;
	}

	void ParticleEmitter::Serialize(Serializer& sl, const char* pszLable/* = "ParticleEmitter"*/)
	{

	}

}
