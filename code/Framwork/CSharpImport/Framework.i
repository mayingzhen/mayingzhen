
%{

#include "Common/Module.h"
#include "Engine/Module.h"
#include "Framwork/Module.h"


%}

%include "../../Code/Framwork/Export.h"
%include "../../Code/Framwork/Component/Component.h"
%include "../../Code/Framwork/Component/AnimPlayComponent.h"
%include "../../Code/Framwork/Component/CollisionComponent.h"
%include "../../Code/Framwork/Component/RigidbodyComponent.h"
%include "../../Code/Framwork/GameObj.h"
%include "../../Code/Framwork/SceneNode.h"


//%template(SafeCastAnimPlayComponent) SafeCast<AnimPlayComponent>;
//%template(SafeCastRigidBodyComponent) SafeCast<RigidbodyComponent>;

