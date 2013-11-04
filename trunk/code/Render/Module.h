#ifndef  _Render_Moudule__H__
#define  _Render_Moudule__H__

#include "WarningPrefix.h"

#include "Render/Export.h"

#include "Render/Light/Light.h"
#include "Render/Light/LightSystem.h"

#include "Render/IRenderDevice/RenderDefine.h"
#include "Render/IRenderDevice/IRenderDevice.h"
#include "Render/IRenderDevice/IndexBuffer.h"
#include "Render/IRenderDevice/VertexBuffer.h"
#include "Render/IRenderDevice/VertexDeclaration.h"

//
#include "Render/Renderable/MeshBatch.h"
#include "Render/Renderable/MeshData.h"
#include "Render/Renderable/Renderable.h"
#include "Render/Renderable/RenderObject.h"
#include "Render/Renderable/RendMesh.h"
#include "Render/Renderable/SpriteBatch.h"


#include "Render/Particle/ParticleEmitter.h"
#include "Render/Particle/ParticleSystem.h"


// Material
#include "Render/Material/Material.h"
#include "Render/Material/MaterialParameter.h"
#include "Render/Material/ShaderProgram.h"
#include "Render/Material/Texture.h"
#include "Render/Material/SamplerState.h"
#include "Render/Material/Technqiue.h"


#include "Render/RenderSystem/RenderSetting.h"
#include "Render/RenderSystem/RenderSystem.h"
#include "Render/RenderSystem/Camera.h"
#include "Render/RenderSystem/RenderContext.h"

#include "Render/RenderScheme/RenderPass.h"
#include "Render/RenderScheme/RenderScheme.h"
#include "Render/RenderScheme/DefferredRender.h"

// Util
#include "Render/Util/LineRender.h"


#include "WarningSuffix.h"


RENDER_API void RenderModuleInit();

RENDER_API void RenderModuleShutdown();



#endif
