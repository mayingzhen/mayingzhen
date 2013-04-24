#ifndef  _Application_Moudule__H__
#define  _Application_Moudule__H__

#define MW_ALIGN_ENUM(str) 
#define DeclareSmartPtr(str) 

#include "Engine/Export.h"

// Event
#include "Engine/Event/Event.h"

// Application
#include "Engine/Application/Input.h"
#include "Engine/Application/Timer.h"
#include "Engine/Application/SceneView.h"
#include "Engine/Application/Application.h"
#include "Engine/Application/ApplicationWin.h"


// Resource
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
#include "Engine/Resource/Serialize/XMLSerialize.h"


// RenderSystem
#include "Engine/RenderSystem/IRenderDevice.h"
#include "Engine/RenderSystem/IRendMesh.h"
#include "Engine/RenderSystem/IRendTexture.h"
#include "Engine/RenderSystem/MeshComponent.h"
#include "Engine/RenderSystem/IndexBuffer.h"
#include "Engine/RenderSystem/VertexBuffer.h"
#include "Engine/RenderSystem/RenderDefine.h"
#include "Engine/RenderSystem/VertexDeclaration.h"
#include "Engine/RenderSystem/PrimtiveStream.h"
#include "Engine/RenderSystem/Material.h"
#include "Engine/RenderSystem/Renderer.h"

// Animation
#include "Engine/Animation/IAnimationDevice.h"
#include "Engine/Animation/IAnimationPlay.h"
#include "Engine/Animation/ISkeleton.h"
#include "Engine/Animation/IAnimationSet.h"
#include "Engine/Animation/SkelMeshComponent.h"


// Physics
#include "Engine/Physics/ICharacterController.h"
#include "Engine/Physics/ICollisionShape.h"
#include "Engine/Physics/IPhysicsDevive.h"
#include "Engine/Physics/IPhysicsObject.h"
#include "Engine/Physics/IPhysicsScene.h"
#include "Engine/Physics/IRigidBody.h"
#include "Engine/Physics/RigidbodyComponent.h"
#include "Engine/Physics/CollisionComponent.h"


// script
#include "Engine/Script/IScriptDevice.h"
#include "Engine/Script/IScriptObject.h"
#include "Engine/Script/EdScriptObject.h"
#include "Engine/Script/ScriptComponent.h"
//#include "Engine/Script/ScriptEventBinder.h"


// Scene
#include "Engine/Scene/Camera.h"
#include "Engine/Scene/Component.h"
#include "Engine/Scene/GameObj.h"
#include "Engine/Scene/Light.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/SceneNode.h"




ENGINE_API void EngineModuleInit();

ENGINE_API void EngineModuleShutdown();

#endif
