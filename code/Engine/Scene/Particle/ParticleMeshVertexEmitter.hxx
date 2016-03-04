#include "S3EngineCommon.h"
#include "ParticleMeshVertexEmitter.h"

namespace ma
{

ParticleMeshVertexEmitter::ParticleMeshVertexEmitter()
:  mUseNormalDirection(false)
{
	mScale = Vector3::UNIT_SCALE;
    mRotate = Quaternion::IDENTITY;
	MESHEMITTER_VERTEX v(Vector3::ZERO, Vector3::UNIT_Z);
	this->SetMesh(&v, 1);
}

ParticleMeshVertexEmitter::~ParticleMeshVertexEmitter(void)
{
}

int ParticleMeshVertexEmitter::Emitt( Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles )
{
    if (!m_bEnabled)
        return 0;

	mParticles.clear();
	uint32 amount = this->GenConstantEmissionCount(now,timeSinceLastCall, nFinalMaxParticles);

    int nVerterAmount = mSrcVertices.size();

	SParticle p;
	for(uint32 i=0; i<amount; ++i)
	{
        int nIndex = (int)(Math::UnitRandom()*(nVerterAmount-1));
		{
			const MESHEMITTER_VERTEX& vert = mSrcVertices[nIndex];
			p.prePos = p.startPos = p.pos = mRotate*(vert.pos*mScale)+mPos;
			p.startTime = now-timeSinceLastCall;
			GenEmission(p);

			if( mUseNormalDirection )
				p.vector = (mRotate*vert.normal)*(mMinVelocity + Randomizer::frand()*(mMaxVelocity-mMinVelocity));
			else
				p.vector = mDirection*(mMinVelocity + Randomizer::frand()*(mMaxVelocity-mMinVelocity));
			{
				Vector3 tgt = p.vector;
                if (mMaxAngleDegrees.z)
                    tgt.rotateXYBy(Radian(Degree((Real)(Randomizer::rand()%(mMaxAngleDegrees.z*2) - mMaxAngleDegrees.z))));

                if(mMaxAngleDegrees.x)
                    tgt.rotateYZBy(Radian(Degree((Real)(Randomizer::rand()%(mMaxAngleDegrees.x*2) - mMaxAngleDegrees.x))));

                if(mMaxAngleDegrees.y)
                    tgt.rotateXZBy(Radian(Degree((Real)(Randomizer::rand()%(mMaxAngleDegrees.y*2) - mMaxAngleDegrees.y))));

				p.vector = tgt;
			}
			p.startVector = p.vector;
			
			mParticles.push_back(p);
		}
	}

	if (mParticles.empty())
	{
		outArray = NULL;
	}
	else
	{
		outArray = &mParticles[0];
	}

	return mParticles.size();
}

void ParticleMeshVertexEmitter::SetPos( const Vector3& pos )
{
	ParticleEmitter::SetPos(pos);
}

void ParticleMeshVertexEmitter::SetDirection( const Vector3& newDirection )
{
	ParticleEmitter::SetDirection(newDirection);
	mRotate = this->CalcRotate();
}

//! Set Mesh to emit particles from
void ParticleMeshVertexEmitter::SetMesh(MESHEMITTER_VERTEX* rgVertices, uint32 nNumVertices)
{
	mSrcVertices.clear();
	for (uint32 i = 0;i< nNumVertices;++i)
	{
		mSrcVertices.push_back(rgVertices[i]);
	}
}

Quaternion ParticleMeshVertexEmitter::CalcRotate() const
{
	Vector3 mUp = mDirection.perpendicular();
	Vector3 mLeft = mUp.crossProduct(mDirection);
	return Quaternion(mLeft.normalisedCopy(), mUp.normalisedCopy(), mDirection.normalisedCopy());
}

void ParticleMeshVertexEmitter::SetScale( const Vector3& vScale )
{
	mScale = vScale;
}

void ParticleMeshVertexEmitter::BeginVertex()
{
	mSrcVertices.clear();
}

void ParticleMeshVertexEmitter::AddVertex( const MESHEMITTER_VERTEX& v )
{
	mSrcVertices.push_back(v);
}

void ParticleMeshVertexEmitter::EndVertex()
{
}

 RefPtr<ParticleMeshVertexEmitter> CreateMeshVertexEmitter()
{
	return new ParticleMeshVertexEmitter();
}
}