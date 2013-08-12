
%{

#include "Common/Module.h"
#include "Engine/Module.h"
#include "Framwork/Module.h"


%}

%include "../../Code/Framwork/Export.h"
%include "../../Code/Framwork/Component/Component.h"
%include "../../Code/Framwork/Component/AnimComponent.h"
%include "../../Code/Framwork/Component/CollisionComponent.h"
%include "../../Code/Framwork/Component/RigidbodyComponent.h"
%include "../../Code/Framwork/GameObj.h"
%include "../../Code/Framwork/SceneNode.h"


//%template(SafeCastAnimComponent) SafeCast<AnimComponent>;
//%template(SafeCastRigidBodyComponent) SafeCast<RigidbodyComponent>;

