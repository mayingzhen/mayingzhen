#include "Engine/stdAfx.h"
#include "Engine/Module.h"

#include "Engine/Input/Input.hxx"
#include "Engine/Timer/Timer.hxx"

#include "Engine/Application/Application.hxx"
#include "Engine/Application/ApplicationWin.hxx"

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

#include "Engine/RenderSystem/IRender.hxx"
#include "Engine/RenderSystem/IRenderDevice.hxx"
#include "Engine/RenderSystem/IRendMesh.hxx"


void EngineModuleInit()
{
}

void EngineModuleShutdown()
{
}

