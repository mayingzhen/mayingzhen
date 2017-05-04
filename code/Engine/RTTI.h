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
		BlendState::RegisterAttribute();
		DepthStencilState::RegisterAttribute();
		RasterizerState::RegisterAttribute();
		Technique::RegisterAttribute();
		SamplerState::RegisterAttribute();
		VertexElement::RegisterAttribute();
	}

	void EngineRTTIShutdown()
	{
	
	}
}



#endif