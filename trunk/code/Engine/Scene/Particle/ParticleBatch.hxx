#include "ParticleBatch.h"
#include "GenerateData.h"

#define PARTICLE_COUNT_MAX                       100
#define PARTICLE_EMISSION_RATE                   10
#define PARTICLE_EMISSION_RATE_TIME_INTERVAL     1000.0f / (float)PARTICLE_EMISSION_RATE

namespace ma
{
	ParticleBatch::ParticleBatch()
		:SpriteBatch(NULL,NULL,0)
	{
		m_pParticles = NULL;
		m_pSpriteTextureCoords = NULL;
		m_nParticleCount = 0;
		m_bStarted = false;
		m_fTimeRunning = 0;
		m_bUpdate = false;
	}

	ParticleBatch::~ParticleBatch()
	{
		SAFE_DELETE_ARRAY(m_pParticles);
		SAFE_DELETE_ARRAY(m_pSpriteTextureCoords);
	}

	void ParticleBatch::Start()
	{
		m_bStarted = true;
	}

	void ParticleBatch::Stop()
	{
		m_bStarted = false;
	}

	void ParticleBatch::Render(Technique* pTech)
	{
		if (!m_bUpdate)
		{
			Calculate(true);
		}

		UpdateRenderable();

		SpriteBatch::Render(pTech);
	}

	BLEND_MODE getTextureBlendingFromString(const char* str)
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


	bool ParticleBatch::isStarted() const
	{
		return m_bStarted;
	}

	bool ParticleBatch::isActive() const
	{
		if (m_bStarted)
			return true;

		ASSERT(m_pParticles);
		bool active = false;
		for (UINT i = 0; i < m_nParticleCount; i++)
		{
			if (m_pParticles[i]._energy > 0)
			{
				active = true;
				break;
			}
		}

		return active;
	}

	void ParticleBatch::LoadSpriteInfo(Properties* pSpriteProp,SpriteTextureInfo& info)
	{
		char path[256];
		if (!pSpriteProp->getPath("path", path))
		{
			GP_ERROR("Failed to load particle emitter: required image file path ('path') is missing.");
			return ;
		}
		info.m_sTexturePath = path;

		const char* blendingString = pSpriteProp->getString("blending");
		info.m_eTextureBlending = getTextureBlendingFromString(blendingString);
		info.m_nSpriteWidth = pSpriteProp->getInt("width");
		info.m_nSpriteHeight = pSpriteProp->getInt("height");
		info.m_bSpriteAnimated = pSpriteProp->getBool("animated");
		info.m_bSpriteLooped = pSpriteProp->getBool("looped");
		info.m_nSpriteFrameCount = pSpriteProp->getInt("frameCount");
		info.m_nSpriteFrameRandomOffset = pSpriteProp->getInt("frameRandomOffset");
		info.m_fSpriteFrameDuration = pSpriteProp->getFloat("frameDuration");

		RefPtr<Texture> pTexture = LoadResourceSync<Texture>(info.m_sTexturePath.c_str());
		ASSERT(pTexture);
		if (pTexture == NULL)
			return;

		SetTexture(pTexture);

		setSpriteFrameCoords( pTexture->GetWidth(), pTexture->GetHeight());

		GetStateBlock().m_eBlendMode = info.m_eTextureBlending;
	}


	void ParticleBatch::LoadParticleEmitInfo(Properties* pProperties,ParticleEmitInfo& info)
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
// 		pProperties->getColor("colorStart", &info.m_colorStart);
// 		pProperties->getColor("colorStartVar", &info.m_colorStartVar);
// 		pProperties->getColor("colorEnd", &info.m_colorEnd);
// 		pProperties->getColor("colorEndVar", &info.m_colorEndVar);

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

	void ParticleBatch::Load(const char* pParticleEmitPath)
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

	void ParticleBatch::setSpriteFrameCoords(int nTextureWidth, int nTextureHeight)
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

		UINT n = 0;
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


	void ParticleBatch::Calculate(bool bWait)
	{
		if (!isActive())
		{
			return;
		}

 		if ( !bWait && !m_csUpdate.TryLock() )
 			return;

		UpdateParticles();

		m_bUpdate = true; 

		m_csUpdate.Unlock();
	}


	void ParticleBatch::EmitOnce(UINT particleCount)
	{
		// Limit particleCount so as not to go over _particleCountMax.
		if (particleCount + m_nParticleCount > m_particleEmitInfo.m_nParticleCountMax)
		{
			particleCount = m_particleEmitInfo.m_nParticleCountMax - m_nParticleCount;
		}

		// Emit the new particles.
		for (UINT i = 0; i < particleCount; i++)
		{
			Particle* p = &m_pParticles[m_nParticleCount];
			p->_visible = true;

			GenerateColor(m_particleEmitInfo.m_colorStart, m_particleEmitInfo.m_colorStartVar, &p->_colorStart);
			GenerateColor(m_particleEmitInfo.m_colorEnd, m_particleEmitInfo.m_colorEndVar, &p->_colorEnd);
			p->_color = p->_colorStart;

			p->_energy = p->_energyStart = GenerateScalar(m_particleEmitInfo.m_nEnergyMin, m_particleEmitInfo.m_nEnergyMax);
			p->_size = p->_sizeStart = GenerateScalar(m_particleEmitInfo.m_fSizeStartMin, m_particleEmitInfo.m_fSizeStartMax);
			p->_sizeEnd = GenerateScalar(m_particleEmitInfo.m_fSizeEndMin, m_particleEmitInfo.m_fSizeEndMax);
			p->_rotationPerParticleSpeed = GenerateScalar(m_particleEmitInfo.m_fRotationPerParticleSpeedMin, m_particleEmitInfo.m_fRotationPerParticleSpeedMax);
			p->_angle = GenerateScalar(0.0f, p->_rotationPerParticleSpeed);
			p->_rotationSpeed = GenerateScalar(m_particleEmitInfo.m_fRotationSpeedMin, m_particleEmitInfo.m_fRotationSpeedMax);

			// Only initial position can be generated within an ellipsoidal domain.
			GenerateVector(m_particleEmitInfo.m_position, m_particleEmitInfo.m_positionVar, &p->_position, m_particleEmitInfo.m_bEllipsoid);
			GenerateVector(m_particleEmitInfo.m_velocity, m_particleEmitInfo.m_velocityVar, &p->_velocity, false);
			GenerateVector(m_particleEmitInfo.m_acceleration, m_particleEmitInfo.m_accelerationVar, &p->_acceleration, false);
			GenerateVector(m_particleEmitInfo.m_rotationAxis, m_particleEmitInfo.m_rotationAxisVar, &p->_rotationAxis, false);

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
			if (p->_rotationSpeed != 0.0f && p->_rotationAxis != Vector3::ZERO )
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


	void ParticleBatch::UpdateParticles()
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
			UINT emitCount = (UINT)(m_fTimeRunning / ftimePerEmission);

			if (emitCount)
			{
				if ((int)ftimePerEmission > 0)
				{
					m_fTimeRunning = fmod(m_fTimeRunning, ftimePerEmission);
				}
				EmitOnce(emitCount);
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
				if (p->_rotationSpeed != 0.0f && p->_rotationAxis != Vector3::ZERO)
				{
					Matrix3 matRotation;
					matRotation.FromAxisAngle(p->_rotationAxis,Radian(p->_rotationSpeed * elapsedSecs));

					p->_velocity = matRotation * p->_velocity;
					p->_acceleration = matRotation * p->_acceleration;
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

// 				p->_color.x = p->_colorStart.x + (p->_colorEnd.x - p->_colorStart.x) * percent;
// 				p->_color.y = p->_colorStart.y + (p->_colorEnd.y - p->_colorStart.y) * percent;
// 				p->_color.z = p->_colorStart.z + (p->_colorEnd.z - p->_colorStart.z) * percent;
// 				p->_color.w = p->_colorStart.w + (p->_colorEnd.w - p->_colorStart.w) * percent;

				p->_size = p->_sizeStart + (p->_sizeEnd - p->_sizeStart) * percent;

				// Handle sprite animations.
				if (m_spriteTextureInfo.m_bSpriteAnimated)
				{
					if (!m_spriteTextureInfo.m_bSpriteLooped)
					{
						float fSpritePercentPerFrame = 1.0f / (float)m_spriteTextureInfo.m_nSpriteFrameCount;
						// The last frame should finish exactly when the particle dies.
						float percentSpent = 0.0f;
						for (UINT i = 0; i < p->_frame; i++)
						{
							percentSpent += fSpritePercentPerFrame;
						}
						p->_timeOnCurrentFrame = percent - percentSpent;
						if (p->_frame < (float)m_spriteTextureInfo.m_nSpriteFrameCount - 1.0f &&
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

	void ParticleBatch::UpdateRenderable()
	{
		if (m_nParticleCount <= 0)
			return;

		ASSERT(m_pParticles);
		ASSERT(m_pSpriteTextureCoords);

		// Set our node's view projection matrix to this emitter's effect.
		if (GetRenderSystem())
		{
			Matrix4 matWorld = this->GetWorldMatrix();
			Matrix4 matVP = GetRenderContext()->GetViewProjMatrix();
			SetProjectionMatrix(matWorld * matVP);
		}

		// Begin sprite batch drawing
		Reset();

		// 2D Rotation.
		static const Vector2 pivot(0.5f, 0.5f);

		// 3D Rotation so that particles always face the camera.
		Matrix4 matView = GetRenderContext()->GetViewMatrix().inverseAffine();

		Vector3 right = Vector3(matView[0][0], matView[0][1], matView[0][2]);
		Vector3 up = Vector3(matView[1][0], matView[1][1], matView[1][2]);

		for (UINT i = 0; i < m_nParticleCount; i++)
		{
			Particle* p = &m_pParticles[i];

			if (p->_visible)
			{
				Draw(p->_position, right, up, p->_size, p->_size,
					m_pSpriteTextureCoords[p->_frame * 4], 
					m_pSpriteTextureCoords[p->_frame * 4 + 1], 
					m_pSpriteTextureCoords[p->_frame * 4 + 2], 
					m_pSpriteTextureCoords[p->_frame * 4 + 3],
					p->_color, pivot, p->_angle);
			}
		}

	}
}


