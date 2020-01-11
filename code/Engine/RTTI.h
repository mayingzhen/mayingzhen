#pragma once

namespace ma
{
	void EngineRTTIInit()
	{
		SceneNode::RegisterAttribute();
		MeshComponent::RegisterAttribute();
		SkinMeshComponent::RegisterAttribute();
		SkyBox::RegisterAttribute();
		Camera::RegisterAttribute();
		Terrain::RegisterAttribute();
		ParticleComponent::RegisterAttribute();
		RENDER_TARGET_BLEND_DESC::RegisterAttribute();
		BlendState::RegisterAttribute();
		DepthStencilState::RegisterAttribute();
		RasterizerState::RegisterAttribute();
		Technique::RegisterAttribute();
		SamplerState::RegisterAttribute();
	}

	void EngineRTTIShutdown()
	{
	
	}
}

