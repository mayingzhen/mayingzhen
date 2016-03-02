#ifndef  _Engine_RTTI__H__
#define  _Engine_RTTI__H__

namespace ma
{
	void EngineRTTIInit()
	{
		SceneNode::RegisterAttribute();
		MeshComponent::RegisterAttribute();
		SkinMeshComponent::RegisterAttribute();
		Camera::RegisterAttribute();
		Terrain::RegisterAttribute();
		RenderState::RegisterAttribute();

		CParticleSystem::RegisterAttribute();
		CParticleEmitter::RegisterAttribute();
		CParticlePointEmitter::RegisterAttribute();
		CParticleSubUVAffector::RegisterAttribute();
	}

	void EngineRTTIShutdown()
	{
	
	}
}



#endif