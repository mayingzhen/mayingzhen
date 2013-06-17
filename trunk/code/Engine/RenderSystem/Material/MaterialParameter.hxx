//#include "Base.h"
#include "MaterialParameter.h"
//#include "Node.h"

namespace ma
{

MaterialParameter::MaterialParameter(const char* name) :
    _type(MaterialParameter::NONE), _count(1), _dynamic(false), _name(name ? name : ""), _uniform(NULL)
{
    clearValue();
}

MaterialParameter::~MaterialParameter()
{
    clearValue();
}

void MaterialParameter::clearValue()
{
    // Release parameters
    switch (_type)
    {
    case MaterialParameter::SAMPLER:
        //if (_value.samplerValue)
        //    const_cast<Sampler*>(_value.samplerValue)->release();
        break;
    case MaterialParameter::SAMPLER_ARRAY:
        if (_value.samplerArrayValue)
        {
            for (unsigned int i = 0; i < _count; ++i)
            {
                //const_cast<Sampler*>(_value.samplerArrayValue[i])->release();
            }
        }
        break;
    default:
        // Ignore all other cases.
        break;
    }

    // Free dynamic data
    if (_dynamic)
    {
        switch (_type)
        {
        case MaterialParameter::FLOAT:
        case MaterialParameter::FLOAT_ARRAY:
        case MaterialParameter::VECTOR2:
        case MaterialParameter::VECTOR3:
        case MaterialParameter::VECTOR4:
        case MaterialParameter::MATRIX:
            SAFE_DELETE_ARRAY(_value.floatPtrValue);
            break;
        case MaterialParameter::INT:
        case MaterialParameter::INT_ARRAY:
            SAFE_DELETE_ARRAY(_value.intPtrValue);
            break;
        case MaterialParameter::METHOD:
            //SAFE_RELEASE(_value.method);
            break;
        case MaterialParameter::SAMPLER_ARRAY:
            SAFE_DELETE_ARRAY(_value.samplerArrayValue);
            break;
        default:
            // Ignore all other cases.
            break;
        }

        _dynamic = false;
        _count = 1;
    }

    memset(&_value, 0, sizeof(_value));
    _type = MaterialParameter::NONE;
}

const char* MaterialParameter::getName() const
{
    return _name.c_str();
}

Sampler* MaterialParameter::getSampler(unsigned int index) const
{
    if (_type == MaterialParameter::SAMPLER)
        return const_cast<Sampler*>(_value.samplerValue);
    if (_type == MaterialParameter::SAMPLER_ARRAY && index < _count)
        return const_cast<Sampler*>(_value.samplerArrayValue[index]);
    return NULL;
}

void MaterialParameter::SetValue(float value)
{
    clearValue();

    _value.floatValue = value;
    _type = MaterialParameter::FLOAT;
}

void MaterialParameter::SetValue(int value)
{
    clearValue();

    _value.intValue = value;
    _type = MaterialParameter::INT;
}

void MaterialParameter::SetValue(const float* values, unsigned int count)
{
    clearValue();

    _value.floatPtrValue = const_cast<float*> (values);
    _count = count;
    _type = MaterialParameter::FLOAT_ARRAY;
}

void MaterialParameter::SetValue(const int* values, unsigned int count)
{
    clearValue();

    _value.intPtrValue = const_cast<int*> (values);
    _count = count;
    _type = MaterialParameter::INT_ARRAY;
}

void MaterialParameter::SetValue(const Vector2& value)
{
    clearValue();

    // Copy data by-value into a dynamic array.
    float* array = new float[2];
    memcpy(array, &value.x, sizeof(float) * 2);

    _value.floatPtrValue = array;
    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::VECTOR2;
}

void MaterialParameter::SetValue(const Vector2* values, unsigned int count)
{
    ASSERT(values);
    clearValue();

    _value.floatPtrValue = const_cast<float*> (&values[0].x);
    _count = count;
    _type = MaterialParameter::VECTOR2;
}

void MaterialParameter::SetValue(const Vector3& value)
{
    clearValue();

    // Copy data by-value into a dynamic array.
    float* array = new float[3];
    memcpy(array, &value.x, sizeof(float) * 3);

    _value.floatPtrValue = array;
    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::VECTOR3;
}

void MaterialParameter::SetValue(const Vector3* values, unsigned int count)
{
    ASSERT(values);
    clearValue();

    _value.floatPtrValue = const_cast<float*> (&values[0].x);
    _count = count;
    _type = MaterialParameter::VECTOR3;
}

void MaterialParameter::SetValue(const Vector4& value)
{
    clearValue();

    // Copy data by-value into a dynamic array.
    float* array = new float[4];
    memcpy(array, &value.x, sizeof(float) * 4);

    _value.floatPtrValue = array;
    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::VECTOR4;
}

void MaterialParameter::SetValue(const Vector4* values, unsigned int count)
{
    ASSERT(values);
    clearValue();

    _value.floatPtrValue = const_cast<float*> (&values[0].x);
    _count = count;
    _type = MaterialParameter::VECTOR4;
}

void MaterialParameter::SetValue(const Matrix4x4& value)
{
    // If this parameter is already storing a single dynamic matrix, no need to clear it.
    if (!(_dynamic && _count == 1 && _type == MaterialParameter::MATRIX && _value.floatPtrValue != NULL))
    {
        clearValue();

        // Allocate a new dynamic matrix.
        _value.floatPtrValue = new float[16];
    }

    memcpy(_value.floatPtrValue, value.m, sizeof(float) * 16);

    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::MATRIX;
}

void MaterialParameter::SetValue(const Matrix4x4* values, unsigned int count)
{
    ASSERT(values);
    clearValue();

    _value.floatPtrValue = const_cast<Matrix4x4&> (values[0]);
    _count = count;
    _type = MaterialParameter::MATRIX;
}

void MaterialParameter::SetValue(const Sampler* sampler)
{
    ASSERT(sampler);
    clearValue();

    //const_cast<Sampler*>(sampler)->IncReference();
    _value.samplerValue = sampler;
    _type = MaterialParameter::SAMPLER;
}

void MaterialParameter::SetValue(const Sampler** samplers, unsigned int count)
{
    ASSERT(samplers);
    clearValue();

    for (unsigned int i = 0; i < count; ++i)
    {
        //const_cast<Sampler*>(samplers[i])->IncReference();
    }
    _value.samplerArrayValue = samplers;
    _count = count;
    _type = MaterialParameter::SAMPLER_ARRAY;
}

Sampler* MaterialParameter::SetValue(const char* texturePath, bool generateMipmaps)
{
    ASSERT(texturePath);
    clearValue();

    Sampler* sampler = Sampler::create(texturePath, generateMipmaps);
    if (sampler)
    {
        _value.samplerValue = sampler;
        _type = MaterialParameter::SAMPLER;
    }
    return sampler;
}

void MaterialParameter::setFloat(float value)
{
    SetValue(value);
}

void MaterialParameter::setFloatArray(const float* values, unsigned int count, bool copy)
{
    ASSERT(values);
    clearValue();

    if (copy)
    {
        _value.floatPtrValue = new float[count];
        memcpy(_value.floatPtrValue, values, sizeof(float) * count);
        _dynamic = true;
    }
    else
    {
        _value.floatPtrValue = const_cast<float*> (values);
    }

    _count = count;
    _type = MaterialParameter::FLOAT_ARRAY;
}

void MaterialParameter::setInt(int value)
{
    SetValue(value);
}

void MaterialParameter::setIntArray(const int* values, unsigned int count, bool copy)
{
    ASSERT(values);
    clearValue();

    if (copy)
    {
        _value.intPtrValue = new int[count];
        memcpy(_value.intPtrValue, values, sizeof(int) * count);
        _dynamic = true;
    }
    else
    {
        _value.intPtrValue = const_cast<int*> (values);
    }

    _count = count;
    _type = MaterialParameter::INT_ARRAY;
}

void MaterialParameter::setVector2(const Vector2& value)
{
    SetValue(value);
}

void MaterialParameter::setVector2Array(const Vector2* values, unsigned int count, bool copy)
{
    ASSERT(values);
    clearValue();

    if (copy)
    {
        _value.floatPtrValue = new float[2 * count];
        memcpy(_value.floatPtrValue, const_cast<float*> (&values[0].x), sizeof(float) * 2 * count);
        _dynamic = true;
    }
    else
    {
        _value.floatPtrValue = const_cast<float*> (&values[0].x);
    }

    _count = count;
    _type = MaterialParameter::VECTOR2;
}

void MaterialParameter::setVector3(const Vector3& value)
{
    SetValue(value);
}

void MaterialParameter::setVector3Array(const Vector3* values, unsigned int count, bool copy)
{
    ASSERT(values);
    clearValue();

    if (copy)
    {
        _value.floatPtrValue = new float[3 * count];
        memcpy(_value.floatPtrValue, const_cast<float*> (&values[0].x), sizeof(float) * 3 * count);
        _dynamic = true;
    }
    else
    {
        _value.floatPtrValue = const_cast<float*> (&values[0].x);
    }

    _count = count;
    _type = MaterialParameter::VECTOR3;
}

void MaterialParameter::setVector4(const Vector4& value)
{
    SetValue(value);
}

void MaterialParameter::setVector4Array(const Vector4* values, unsigned int count, bool copy)
{
    ASSERT(values);
    clearValue();

    if (copy)
    {
        _value.floatPtrValue = new float[4 * count];
        memcpy(_value.floatPtrValue, const_cast<float*> (&values[0].x), sizeof(float) * 4 * count);
        _dynamic = true;
    }
    else
    {
        _value.floatPtrValue = const_cast<float*> (&values[0].x);
    }

    _count = count;
    _type = MaterialParameter::VECTOR4;
}

void MaterialParameter::setMatrix(const Matrix4x4& value)
{
    SetValue(value);
}

void MaterialParameter::setMatrixArray(const Matrix4x4* values, unsigned int count, bool copy)
{
    ASSERT(values);
    clearValue();

    if (copy)
    {
        _value.floatPtrValue = new float[16 * count];
        memcpy(_value.floatPtrValue, const_cast<Matrix4x4&> (values[0]).m, sizeof(float) * 16 * count);
        _dynamic = true;
    }
    else
    {
        //_value.floatPtrValue = const_cast<Matrix4x4&> (values[0]).m;
    }

    _count = count;
    _type = MaterialParameter::MATRIX;
}

Sampler* MaterialParameter::setSampler(const char* texturePath, bool generateMipmaps)
{
    return SetValue(texturePath, generateMipmaps);
}

void MaterialParameter::setSampler(const Sampler* value)
{
    SetValue(value);
}

void MaterialParameter::setSamplerArray(const Sampler** values, unsigned int count, bool copy)
{
    ASSERT(values);
    clearValue();

    if (copy)
    {
        _value.samplerArrayValue = new const Sampler*[count];
        memcpy(_value.samplerArrayValue, values, sizeof(Sampler*) * count);
        _dynamic = true;
    }
    else
    {
        _value.samplerArrayValue = values;
    }

    for (unsigned int i = 0; i < count; ++i)
    {
        //const_cast<Sampler*>(_value.samplerArrayValue[i])->IncReference();
    }

    _count = count;
    _type = MaterialParameter::SAMPLER_ARRAY;
}

void MaterialParameter::bind(ShaderProgram* effect)
{
    ASSERT(effect);

    // If we had a Uniform cached that is not from the passed in effect,
    // we need to update our uniform to point to the new effect's uniform.
    if (!_uniform || _uniform->GetEffect() != effect)
    {
        _uniform = effect->GetUniform(_name.c_str());

        if (!_uniform)
        {
            // This parameter was not found in the specified effect, so do nothing.
            //GP_WARN("Warning: Material parameter '%s' not found in effect '%s'.", _name.c_str(), effect->GetId());
            return;
        }
    }

    switch (_type)
    {
    case MaterialParameter::FLOAT:
        effect->SetValue(_uniform, _value.floatValue);
        break;
    case MaterialParameter::FLOAT_ARRAY:
        effect->SetValue(_uniform, _value.floatPtrValue, _count);
        break;
    case MaterialParameter::INT:
        effect->SetValue(_uniform, _value.intValue);
        break;
    case MaterialParameter::INT_ARRAY:
        effect->SetValue(_uniform, _value.intPtrValue, _count);
        break;
    case MaterialParameter::VECTOR2:
        effect->SetValue(_uniform, reinterpret_cast<Vector2*>(_value.floatPtrValue), _count);
        break;
    case MaterialParameter::VECTOR3:
        effect->SetValue(_uniform, reinterpret_cast<Vector3*>(_value.floatPtrValue), _count);
        break;
    case MaterialParameter::VECTOR4:
        effect->SetValue(_uniform, reinterpret_cast<Vector4*>(_value.floatPtrValue), _count);
        break;
    case MaterialParameter::MATRIX:
        effect->SetValue(_uniform, reinterpret_cast<Matrix4x4*>(_value.floatPtrValue), _count);
        break;
    case MaterialParameter::SAMPLER:
        effect->SetValue(_uniform, _value.samplerValue);
        break;
    case MaterialParameter::SAMPLER_ARRAY:
        effect->SetValue(_uniform, _value.samplerArrayValue, _count);
        break;
    case MaterialParameter::METHOD:
        ASSERT(_value.method);
        _value.method->SetValue(effect);
        break;
    default:
        GP_ERROR("Unsupported material parameter type (%d).", _type);
        break;
    }
}

// void MaterialParameter::bindValue(Node* node, const char* binding)
// {
//     ASSERT(binding);
// 
//     if (strcmp(binding, "&Node::getBackVector") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getBackVector);
//     }
//     else if (strcmp(binding, "&Node::getDownVector") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getDownVector);
//     }
//     else if (strcmp(binding, "&Node::getTranslationWorld") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getTranslationWorld);
//     }
//     else if (strcmp(binding, "&Node::getTranslationView") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getTranslationView);
//     }
//     else if (strcmp(binding, "&Node::getForwardVector") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getForwardVector);
//     }
//     else if (strcmp(binding, "&Node::getForwardVectorWorld") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getForwardVectorWorld);
//     }
//     else if (strcmp(binding, "&Node::getForwardVectorView") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getForwardVectorView);
//     }
//     else if (strcmp(binding, "&Node::getLeftVector") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getLeftVector);
//     }
//     else if (strcmp(binding, "&Node::getRightVector") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getRightVector);
//     }
//     else if (strcmp(binding, "&Node::getRightVectorWorld") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getRightVectorWorld);
//     }
//     else if (strcmp(binding, "&Node::getUpVector") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getUpVector);
//     }
//     else if (strcmp(binding, "&Node::getUpVectorWorld") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getUpVectorWorld);
//     }
//     else if (strcmp(binding, "&Node::getActiveCameraTranslationWorld") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getActiveCameraTranslationWorld);
//     }
//     else if (strcmp(binding, "&Node::getActiveCameraTranslationView") == 0)
//     {
//         bindValue<Node, Vector3>(node, &Node::getActiveCameraTranslationView);
//     }
//     else if (strcmp(binding, "&Node::getScaleX") == 0)
//     {
//         bindValue<Node, float>(node, &Node::getScaleX);
//     }
//     else if (strcmp(binding, "&Node::getScaleY") == 0)
//     {
//         bindValue<Node, float>(node, &Node::getScaleY);
//     }
//     else if (strcmp(binding, "&Node::getScaleZ") == 0)
//     {
//         bindValue<Node, float>(node, &Node::getScaleZ);
//     }
//     else if (strcmp(binding, "&Node::getTranslationX") == 0)
//     {
//         bindValue<Node, float>(node, &Node::getTranslationX);
//     }
//     else if (strcmp(binding, "&Node::getTranslationY") == 0)
//     {
//         bindValue<Node, float>(node, &Node::getTranslationY);
//     }
//     else if (strcmp(binding, "&Node::getTranslationZ") == 0)
//     {
//         bindValue<Node, float>(node, &Node::getTranslationZ);
//     }
//     else
//     {
//         GP_ERROR("Unsupported material parameter binding '%s'.", binding);
//     }
// }

unsigned int MaterialParameter::getAnimationPropertyComponentCount(int propertyId) const
{
    switch (propertyId)
    {
        case ANIMATE_UNIFORM:
        {
            switch (_type)
            {
                // These types don't support animation.
                case NONE:
                case MATRIX:
                case SAMPLER:
                case SAMPLER_ARRAY:
                case METHOD:
                    return 0;
                case FLOAT:
                case FLOAT_ARRAY:
                case INT:
                case INT_ARRAY:
                    return _count;
                case VECTOR2:
                    return 2 * _count;
                case VECTOR3:
                    return 3 * _count;
                case VECTOR4:
                    return 4 * _count;
                default:
                    GP_ERROR("Unsupported material parameter type (%d).", _type);
                    return 0;
            }
        }
        break;
    }

    return 0;
}

// void MaterialParameter::getAnimationPropertyValue(int propertyId, AnimationValue* value)
// {
//     ASSERT(value);
//     switch (propertyId)
//     {
//         case ANIMATE_UNIFORM:
//         {
//             switch (_type)
//             {
//                 case FLOAT:
//                     value->setFloat(0, _value.floatValue);
//                     break;
//                 case FLOAT_ARRAY:
//                     ASSERT(_value.floatPtrValue);
//                     for (unsigned int i = 0; i < _count; i++)
//                     {
//                         value->setFloat(i, _value.floatPtrValue[i]);
//                     }
//                     break;
//                 case INT:
//                     value->setFloat(0, _value.intValue);
//                     break;
//                 case INT_ARRAY:
//                     ASSERT(_value.intPtrValue);
//                     for (unsigned int i = 0; i < _count; i++)
//                     {
//                         value->setFloat(i, _value.intPtrValue[i]);
//                     }
//                     break;
//                 case VECTOR2:
//                     value->setFloats(0, _value.floatPtrValue, _count * 2);
//                     break;
//                 case VECTOR3:
//                     value->setFloats(0, _value.floatPtrValue, _count * 3);
//                     break;
//                 case VECTOR4:
//                     value->setFloats(0, _value.floatPtrValue, _count * 4);
//                     break;
//                 case NONE:
//                 case MATRIX:
//                 case METHOD:
//                 case SAMPLER:
//                 case SAMPLER_ARRAY:
//                     // Unsupported material parameter types for animation.
//                     break;
//                 default:
//                     GP_ERROR("Unsupported material parameter type (%d).", _type);
//                     break;
//             }
//         }
//         break;
//     }
// }

// void MaterialParameter::setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight)
// {
//     ASSERT(value);
//     ASSERT(blendWeight >= 0.0f && blendWeight <= 1.0f);
// 
//     switch (propertyId)
//     {
//         case ANIMATE_UNIFORM:
//         {
//             switch (_type)
//             {
//                 case FLOAT:
//                     _value.floatValue = Curve::lerp(blendWeight, _value.floatValue, value->getFloat(0));
//                     break;
//                 case FLOAT_ARRAY:
//                     applyAnimationValue(value, blendWeight, 1);
//                     break;
//                 case INT:
//                     _value.intValue = Curve::lerp(blendWeight, _value.intValue, value->getFloat(0));
//                     break;
//                 case INT_ARRAY:
//                     ASSERT(_value.intPtrValue);
//                     for (unsigned int i = 0; i < _count; i++)
//                         _value.intPtrValue[i] = Curve::lerp(blendWeight, _value.intPtrValue[i], value->getFloat(i));
//                     break;
//                 case VECTOR2:
//                     applyAnimationValue(value, blendWeight, 2);
//                     break;
//                 case VECTOR3:
//                     applyAnimationValue(value, blendWeight, 3);
//                     break;
//                 case VECTOR4:
//                     applyAnimationValue(value, blendWeight, 4);
//                     break;
//                 case NONE:
//                 case MATRIX:
//                 case METHOD:
//                 case SAMPLER:
//                 case SAMPLER_ARRAY:
//                     // Unsupported material parameter types for animation.
//                     break;
//                 default:
//                     GP_ERROR("Unsupported material parameter type (%d).", _type);
//                     break;
//             }
//         }
//         break;
//     }
// }

// void MaterialParameter::applyAnimationValue(AnimationValue* value, float blendWeight, int components)
// {
//     ASSERT(value);
//     ASSERT(_value.floatPtrValue);
// 
//     unsigned int count = _count * components;
//     for (unsigned int i = 0; i < count; i++)
//         _value.floatPtrValue[i] = Curve::lerp(blendWeight, _value.floatPtrValue[i], value->getFloat(i));
// }

void MaterialParameter::cloneInto(MaterialParameter* materialParameter) const
{
    ASSERT(materialParameter);
    materialParameter->_type = _type;
    materialParameter->_count = _count;
    materialParameter->_dynamic = _dynamic;
    materialParameter->_uniform = _uniform;
    switch (_type)
    {
    case NONE:
        break;
    case FLOAT:
        materialParameter->SetValue(_value.floatValue);
        break;
    case FLOAT_ARRAY:
        materialParameter->SetValue(_value.floatPtrValue, _count);
        break;
    case INT:
        materialParameter->SetValue(_value.intValue);
        break;
    case INT_ARRAY:
        materialParameter->SetValue(_value.intPtrValue, _count);
        break;
    case VECTOR2:
    {
        Vector2* value = reinterpret_cast<Vector2*>(_value.floatPtrValue);
        if (_count == 1)
        {
            ASSERT(value);
            materialParameter->SetValue(*value);
        }
        else
        {
            materialParameter->SetValue(value, _count);
        }
        break;
    }   
    case VECTOR3:
    {
        Vector3* value = reinterpret_cast<Vector3*>(_value.floatPtrValue);
        if (_count == 1)
        {
            ASSERT(value);
            materialParameter->SetValue(*value);
        }
        else
        {
            materialParameter->SetValue(value, _count);
        }
        break;
    }
    case VECTOR4:
    {
        Vector4* value = reinterpret_cast<Vector4*>(_value.floatPtrValue);
        if (_count == 1)
        {
            ASSERT(value);
            materialParameter->SetValue(*value);
        }
        else
        {
            materialParameter->SetValue(value, _count);
        }
        break;
    }
    case MATRIX:
    {
        Matrix4x4* value = reinterpret_cast<Matrix4x4*>(_value.floatPtrValue);
        if (_count == 1)
        {
            ASSERT(value);
            materialParameter->SetValue(*value);
        }
        else
        {
            materialParameter->SetValue(value, _count);
        }
        break;
    }
    case SAMPLER:
        materialParameter->SetValue(_value.samplerValue);
        break;
    case SAMPLER_ARRAY:
        materialParameter->SetValue(_value.samplerArrayValue, _count);
        break;
    case METHOD:
        materialParameter->_value.method = _value.method;
        ASSERT(materialParameter->_value.method);
        //materialParameter->_value.method->IncReference();
        break;
    default:
        GP_ERROR("Unsupported material parameter type(%d).", _type);
        break;
    }
    
    //NodeCloneContext context;
    //this->AnimationTarget::cloneInto(materialParameter, context);
}

MaterialParameter::MethodBinding::MethodBinding(MaterialParameter* param) :
    _parameter(param), _autoBinding(false)
{
}

}
