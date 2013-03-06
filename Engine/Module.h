#ifndef  _Application_Moudule__H__
#define  _Application_Moudule__H__

#include "Engine/Export.h"
#include "Engine/Input/Input.h"
#include "Engine/Timer/Timer.h"

#include "Engine/Application/SceneView.h"
#include "Engine/Application/Application.h"
#include "Engine/Application/ApplicationWin.h"

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

#include "Engine/RenderSystem/IRender.h"
#include "Engine/RenderSystem/IRenderDevice.h"
#include "Engine/RenderSystem/IRendMesh.h"
#include "Engine/RenderSystem/IRendTexture.h"
#include "Engine/RenderSystem/RenderQueue.h"



ENGINE_API void EngineModuleInit();

ENGINE_API void EngineModuleShutdown();

#endif
