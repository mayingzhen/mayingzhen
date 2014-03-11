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
%include "../../code/Common/Export.h"
%include "../../code/Common/Macro.h"
%include "../../code/Common/BaseType.h"
%include "../../code/Common/Math/MathBase.h"
%include "../../code/Common/Math/Math.h"
%include "../../code/Common/AABB.h"
%include "../../code/Common/Rectangle.h"
%include "../../code/Common/Log.h"
%include "../../code/Common/Referenced.h"
%include "../../code/Common/RefPtr.h"

//%template(SafeCastAnimComponent) SafeCast<AnimComponent>;


