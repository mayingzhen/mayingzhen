
%{

#include "Engine/Module.h"

%}

%include "../../Code/Engine/Export.h"

%include "../../code/Engine/RTTI/Attribute.h"
%include "../../code/Engine/RTTI/Context.h"
%include "../../code/Engine/RTTI/Object.h"
%include "../../code/Engine/RTTI/RTTIClass.h"


%include "../../Code/Engine/Input/Input.h"
%include "../../Code/Engine/Time/Timer.h"

%include "../../Code/Engine/Resource/Stream.h"
%include "../../Code/Engine/Resource/FileInfo.h"

%include "../../Code/Engine/Resource/Resource.h"
%include "../../Code/Engine/Resource/Serialize/Serializer.h"

%include "../../Code/Engine/Scene/Component.h"
%include "../../Code/Engine/Scene/SceneNode.h"

%include "../../Code/Engine/Physics/ICharacterController.h"
%include "../../Code/Engine/Physics/ICollisionShape.h"
%include "../../Code/Engine/Physics/IPhysicsSystem.h"
%include "../../Code/Engine/Physics/IRigidBody.h"
%include "../../Code/Engine/Physics/IPhysicsJoint.h"


//%template(SafeCastAnimComponent) SafeCast<AnimComponent>;
//%template(SafeCastRigidBodyComponent) SafeCast<RigidbodyComponent>;






