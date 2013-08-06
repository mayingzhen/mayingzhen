%{
#include "Common/Module.h"
%}

%feature("ref") RefCountObject "$this->IncReference();"
%feature("unref") RefCountObject "$this->DecReference();"

%rename(Add) Vector3::operator +;
%rename(Subtract) Vector3::operator -;
%rename(Multiply) Vector3::operator *;
%rename(Divide) Vector3::operator /;
%rename(Equals) Vector3::operator ==;

// 相对于Swig目录
%include "../../code/Common/Export.h"
%include "../../code/Common/Macro.h"
%include "../../code/Common/BaseType.h"
%include "../../code/Common/RTTI/Object.h"
%include "../../code/Common/RTTI/RTTIClass.h"
%include "../../code/Common/Math/MathBase.h"
%include "../../code/Common/Math/Math.h"
%include "../../code/Common/AABB.h"
%include "../../code/Common/Rectangle.h"
%include "../../code/Common/Log.h"