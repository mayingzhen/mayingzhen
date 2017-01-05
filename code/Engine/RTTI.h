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
		Technique::RegisterAttribute();
		SamplerState::RegisterAttribute();
	}

	void EngineRTTIShutdown()
	{
	
	}
}



#endif