
%include "typemaps.i"

%apply float *INOUT { float* pAngle}

%ignore S3LKeyFrameAnimation::AddKeyFrame;
%ignore S3LKeyFrameAnimation::SampleTrack;

%include "../../../Extern/S3AnimationSDK/code/Animation/Common/xmCommonConfig.h"
//%include "../../../Extern/S3AnimationSDK/code/Animation/Common/Math/xmBaseType.h"
%include "../../../Extern/S3AnimationSDK/code/Animation/Common/Math/xmMathBase.h"
%include "../../../Extern/S3AnimationSDK/code/Animation/Common/Math/xmMath.h"
%include "../../../Extern/S3AnimationSDK/code/Animation/Common/Math/xmAxisAlignedBox.h"
%include "../../../Extern/S3AnimationSDK/code/Animation/Common/RTTI/S3AClass.h"
%include "../../../Extern/S3AnimationSDK/code/Animation/Common/RTTI/S3AObject.h" 
%include "../../../Extern/S3AnimationSDK/code/Animation/Common/Name/S3AName.h"




