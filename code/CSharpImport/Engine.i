
%{

#include "Engine/Module.h"

%}


%ignore ma::Time::Time;
%ignore ma::Time::~Time;
%ignore ma::Time::Reset;
%ignore ma::Time::GetMillisceonds;
%ignore ma::Time::GetFrameTotalTime;
%ignore ma::Time::Sleep;
%ignore ma::SetTimer;
%ignore ma::g_pTimer;
%include "../../Code/Engine/Time/Timer.h"


#define DECL_OBJECT(x)
%ignore ma::SceneNode::RegisterAttribute;
%ignore ma::SceneNode::Serialize;
%ignore ma::SceneNode::SetName;
%ignore ma::Component::SetSceneNode;

%include "../../Code/Engine/Scene/Component.h"
%include "../../Code/Engine/Scene/SceneNode.h"








