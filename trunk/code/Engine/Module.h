#ifndef  _Application_Moudule__H__
#define  _Application_Moudule__H__

#define MW_ALIGN_ENUM(str) 

#include "WarningPrefix.h"

#include "Engine/Export.h"

// Event
#include "Engine/Event/Event.h"

// Platform
#include "Engine/Application/Timer.h"
#include "Engine/Application/Platform.h"
#include "Engine/Application/Game.h"



// Resource
#include "Engine/Resource/Properties.h"
#include "Engine/Resource/Stream.h"
#include "Engine/Resource/FileSystem.h"
#include "Engine/Resource/Resource.h"
#include "Engine/Resource/MeshData.h"
#include "Engine/Resource/AnimationData.h"
#include "Engine/Resource/SkeletonData.h"
#include "Engine/Resource/ResourceBuilder.h"
#include "Engine/Resource/Serialize/tinyxml/tinyxml.h"
#include "Engine/Resource/Serialize/SerializeListener.h"
#include "Engine/Resource/Serialize/BinaryInputArchive.h"
#include "Engine/Resource/Serialize/BinaryOutputArchive.h"
#include "Engine/Resource/Serialize/StringConverter.h"
#include "Engine/Resource/Serialize/XMLInputArchive.h"
#include "Engine/Resource/Serialize/XMLOutputArchive.h"


// RenderSystem
#include "Engine/RenderSystem/RenderDefine.h"
#include "Engine/RenderSystem/IRenderDevice.h"
#include "Engine/RenderSystem/RendMesh.h"
#include "Engine/RenderSystem/IndexBuffer.h"
#include "Engine/RenderSystem/VertexBuffer.h"
#include "Engine/RenderSystem/VertexDeclaration.h"
#include "Engine/RenderSystem/Renderable.h"
#include "Engine/RenderSystem/MeshBatch.h"
#include "Engine/RenderSystem/ParticleEmitter.h"
#include "Engine/RenderSystem/SpriteBatch.h"
#include "Engine/RenderSystem/RenderTarget.h"

#include "Engine/RenderSystem/Material/Material.h"
#include "Engine/RenderSystem/Material/MaterialParameter.h"
#include "Engine/RenderSystem/Material/ShaderProgram.h"
#include "Engine/RenderSystem/Material/Texture.h"



// Animation
#include "Engine/Animation/IAnimationDevice.h"
#include "Engine/Animation/IAnimationPlay.h"
#include "Engine/Animation/ISkeleton.h"
#include "Engine/Animation/IAnimationSet.h"


// Physics
#include "Engine/Physics/ICharacterController.h"
#include "Engine/Physics/ICollisionShape.h"
#include "Engine/Physics/IPhysicsDevive.h"
#include "Engine/Physics/IPhysicsObject.h"
#include "Engine/Physics/IPhysicsScene.h"
#include "Engine/Physics/IRigidBody.h"


// script
#include "Engine/Script/IScriptDevice.h"
#include "Engine/Script/IScriptObject.h"
#include "Engine/Script/EdScriptObject.h"
//#include "Engine/Script/ScriptEventBinder.h"


// Scene
#include "Engine/Scene/Camera.h"
//#include "Engine/Scene/Light.h"


// Input
#include "Engine/Input/Gesture.h"
#include "Engine/Input/Keyboard.h"
#include "Engine/Input/Mouse.h"
#include "Engine/Input/Touch.h"
//#include "Engine/Input/Gamepad.h"
//#include "Engine/Input/Joystick.h"

// UI
// #include "Engine/UI/Control.h"
// #include "Engine/UI/Layout.h"
// #include "Engine/UI/FlowLayout.h"
// #include "Engine/UI/AbsoluteLayout.h"
// #include "Engine/UI/VerticalLayout.h"
// #include "Engine/UI/CheckBox.h"
// #include "Engine/UI/Container.h"
// #include "Engine/UI/Font.h"
// #include "Engine/UI/Form.h"
// #include "Engine/UI/Label.h"
// #include "Engine/UI/Button.h"
// #include "Engine/UI/RadioButton.h"
// #include "Engine/UI/ImageControl.h"
// #include "Engine/UI/Theme.h"
// #include "Engine/UI/ThemeStyle.h"
// #include "Engine/UI/Slider.h"
// #include "Engine/UI/TextBox.h"

//#include "Engine/UI/Bundle.h"


#include "WarningSuffix.h"


ENGINE_API void EngineModuleInit();

ENGINE_API void EngineModuleShutdown();

#endif
