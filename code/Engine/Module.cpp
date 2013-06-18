#include "Engine/stdAfx.h"
#include "Engine/Module.h"


// Platform
#include "Engine/Application/Timer.hxx"
#include "Engine/Application/Platform.hxx"
#include "Engine/Application/Game.hxx"


// Event
#include "Engine/Event/Event.hxx"


// Resource
#include "Engine/Resource/Properties.hxx"
#include "Engine/Resource/FileSystem.hxx"
#include "Engine/Resource/Resource.hxx"
#include "Engine/Resource/MeshData.hxx"
#include "Engine/Resource/AnimationData.hxx"
#include "Engine/Resource/SkeletonData.hxx"
#include "Engine/Resource/ResourceBuilder.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinyxml.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinystr.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinyxmlerror.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinyxmlparser.hxx"
#include "Engine/Resource/Serialize/SerializeListener.hxx"
#include "Engine/Resource/Serialize/BinaryInputArchive.hxx"
#include "Engine/Resource/Serialize/BinaryOutputArchive.hxx"
#include "Engine/Resource/Serialize/XMLInputArchive.hxx"
#include "Engine/Resource/Serialize/XMLOutputArchive.hxx"


// RenderSystem
#include "Engine/RenderSystem/IRenderDevice.hxx"
#include "Engine/RenderSystem/RendMesh.hxx"
#include "Engine/RenderSystem/VertexDeclaration.hxx"
#include "Engine/RenderSystem/MeshBatch.hxx"
#include "Engine/RenderSystem/ParticleEmitter.hxx"
#include "Engine/RenderSystem/SpriteBatch.hxx"
#include "Engine/RenderSystem/RenderTarget.hxx"

#include "Engine/RenderSystem/Material/Material.hxx"
#include "Engine/RenderSystem/Material/MaterialParameter.hxx"
#include "Engine/RenderSystem/Material/ShaderProgram.hxx"
#include "Engine/RenderSystem/Material/Texture.hxx"


// Animation
#include "Engine/Animation/IAnimationDevice.hxx"


// Physics
#include "Engine/Physics/IPhysicsDevive.hxx"


// script
#include "Engine/Script/IScriptDevice.hxx"
#include "Engine/Script/EdScriptObject.hxx"
//#include "Engine/Script/ScriptEventBinder.hxx"


// Scene
#include "Engine/Scene/Camera.hxx"
//#include "Engine/Scene/Light.hxx"



// Input
//#include "Engine/Input/Joystick.hxx"
//#include "Engine/Input/Gamepad.hxx"



// UI
#include "Engine/UI/Control.hxx"
#include "Engine/UI/Layout.hxx"
#include "Engine/UI/FlowLayout.hxx"
#include "Engine/UI/AbsoluteLayout.hxx"
#include "Engine/UI/VerticalLayout.hxx"
#include "Engine/UI/CheckBox.hxx"
#include "Engine/UI/Container.hxx"
#include "Engine/UI/Font.hxx"
#include "Engine/UI/Form.hxx"
#include "Engine/UI/Label.hxx"
#include "Engine/UI/Button.hxx"
#include "Engine/UI/RadioButton.hxx"
#include "Engine/UI/ImageControl.hxx"
#include "Engine/UI/Theme.hxx"
#include "Engine/UI/ThemeStyle.hxx"
#include "Engine/UI/Slider.hxx"
#include "Engine/UI/TextBox.hxx"

#include "Engine/UI/Bundle.hxx"


using namespace ma;

// RTTI
#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();}
#include <Engine/RTTIDecl.h>
#undef RTTI_DECL


void EngineModuleInit()
{
	#define RTTI_DECL(ClassType) ClassType::StaticInitClass();
	#include <Engine/RTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Engine/RTTIDecl.h>
	#undef RTTI_DECL

	//RenderState::initialize();
}

void EngineModuleShutdown()
{
	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().UnRegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Engine/RTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ClassType::StaticShutdownClass();
	#include <Engine/RTTIDecl.h>
	#undef RTTI_DECL

	//RenderState::finalize()
}
