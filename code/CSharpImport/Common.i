%{
#include "Common/Module.h"
%}

%feature("ref") Referenced "$this->Ref();"
%feature("unref") Referenced "$this->Unref();"

%rename(Add) Vector3::operator +;
%rename(Subtract) Vector3::operator -;
%rename(Multiply) Vector3::operator *;
%rename(Divide) Vector3::operator /;
%rename(Equals) Vector3::operator ==;

// 相对于Swig目录
%include "../../code/Common/Macro.h"
%include "../../code/Common/BaseType.h"
%include "../../code/Common/Log.h"
%include "../../code/Common/Referenced.h"
%include "../../code/Common/RefPtr.h"

// Math
%include "../../code/Common/Math/Angle.h"
%include "../../code/Common/Math/GUID.h"
%include "../../code/Common/Math/Math.h"
%include "../../code/Common/Math/AABB.h"
%include "../../code/Common/Math/ColourValue.h"
%include "../../code/Common/Math/Polygon.h"
%include "../../code/Common/Math/ConvexBody.h"
%include "../../code/Common/Math/Matrix3.h"
%include "../../code/Common/Math/Matrix4.h"
%include "../../code/Common/Math/Plane.h"
%include "../../code/Common/Math/Quaternion.h"
%include "../../code/Common/Math/Ray.h"
%include "../../code/Common/Math/Sphere.h"
%include "../../code/Common/Math/Vector2.h"
%include "../../code/Common/Math/Vector3.h"
%include "../../code/Common/Math/Vector4.h"
%include "../../code/Common/Math/Transform.h"
%include "../../code/Common/Math/Any.h"
%include "../../code/Common/Math/DualQuaternion.h"
%include "../../code/Common/Math/Rectangle.h"



