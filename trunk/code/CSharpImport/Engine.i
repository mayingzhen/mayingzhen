
%{

#include "Engine/Module.h"

%}

%include "../../Code/Engine/Export.h"

%include "../../code/Engine/RTTI/Object.h"
%include "../../code/Engine/RTTI/RTTIClass.h"

%include "../../Code/Engine/Input/Input.h"
%include "../../Code/Engine/Application/Timer.h"

%include "../../Code/Engine/Resource/Stream.h"
%include "../../Code/Engine/Resource/DataStream.h"
%include "../../Code/Engine/Resource/AAssetFileStream.h"
%include "../../Code/Engine/Resource/FileStream.h"
%include "../../Code/Engine/Resource/FileSystem.h"

%include "../../Code/Engine/Resource/Resource.h"
%include "../../Code/Engine/Resource/Serialize/Serializer.h"

%include "../../Code/Engine/EntitySystem/SceneNode.h"
%include "../../Code/Engine/EntitySystem/Component.h"
%include "../../Code/Engine/EntitySystem/GameObj.h"

%include "../../Code/Engine/Animation/IAnimationSystem.h"
%include "../../Code/Engine/Animation/IAnimationObject.h"
%include "../../Code/Engine/Animation/IAnimationSet.h"

%include "../../Code/Engine/Physics/ICharacterController.h"
%include "../../Code/Engine/Physics/ICollisionShape.h"
%include "../../Code/Engine/Physics/IPhysicsSystem.h"
%include "../../Code/Engine/Physics/IPhysicsObject.h"
%include "../../Code/Engine/Physics/IRigidBody.h"
%include "../../Code/Engine/Physics/IPhysicsJoint.h"


//%template(SafeCastAnimComponent) SafeCast<AnimComponent>;
//%template(SafeCastRigidBodyComponent) SafeCast<RigidbodyComponent>;






