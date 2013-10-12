
%{

#include "Common/Module.h"
#include "Engine/Module.h"
#include "EntitySystem/Module.h"


%}

%include "../../Code/EntitySystem/Export.h"
%include "../../Code/EntitySystem/SceneNode.h"
%include "../../Code/EntitySystem/Component/Component.h"
%include "../../Code/EntitySystem/Component/AnimComponent.h"
%include "../../Code/EntitySystem/Component/CollisionComponent.h"
%include "../../Code/EntitySystem/Component/RigidbodyComponent.h"
%include "../../Code/EntitySystem/GameObj.h"



//%template(SafeCastAnimComponent) SafeCast<AnimComponent>;
//%template(SafeCastRigidBodyComponent) SafeCast<RigidbodyComponent>;

