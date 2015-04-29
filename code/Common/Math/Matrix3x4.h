//
// Copyright (c) 2008-2014 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef __Matrix3x4_H__
#define __Matrix3x4_H__

#include "Matrix4.h"
#include "Matrix3.h"

namespace ma
{

/// 3x4 matrix for scene node transform calculations.
class Matrix3x4
{
public:
    /// Construct an identity matrix.
    Matrix3x4() :
        m00_(1.0f),
        m01_(0.0f),
        m02_(0.0f),
        m03_(0.0f),
        m10_(0.0f),
        m11_(1.0f),
        m12_(0.0f),
        m13_(0.0f),
        m20_(0.0f),
        m21_(0.0f),
        m22_(1.0f),
        m23_(0.0f)
    {
    }
    
    /// Copy-construct from another matrix.
    Matrix3x4(const Matrix3x4& matrix) :
        m00_(matrix.m00_),
        m01_(matrix.m01_),
        m02_(matrix.m02_),
        m03_(matrix.m03_),
        m10_(matrix.m10_),
        m11_(matrix.m11_),
        m12_(matrix.m12_),
        m13_(matrix.m13_),
        m20_(matrix.m20_),
        m21_(matrix.m21_),
        m22_(matrix.m22_),
        m23_(matrix.m23_)
    {
    }
    
    /// Copy-construct from a 3x3 matrix and set the extra elements to identity.
    Matrix3x4(const Matrix3& matrix) :
        m00_(matrix.m[0][0]),
        m01_(matrix.m[0][1]),
        m02_(matrix.m[0][2]),
        m03_(0.0f),
        m10_(matrix.m[1][0]),
        m11_(matrix.m[1][1]),
        m12_(matrix.m[1][2]),
        m13_(0.0f),
        m20_(matrix.m[2][0]),
        m21_(matrix.m[2][1]),
        m22_(matrix.m[2][2]),
        m23_(0.0f)
    {
    }
    
    /// Copy-construct from a 4x4 matrix which is assumed to contain no projection.
    Matrix3x4(const Matrix4& matrix) :
        m00_(matrix.m[0][0]),
        m01_(matrix.m[0][1]),
        m02_(matrix.m[0][2]),
        m03_(matrix.m[0][3]),
        m10_(matrix.m[1][0]),
        m11_(matrix.m[1][1]),
        m12_(matrix.m[1][2]),
        m13_(matrix.m[1][3]),
        m20_(matrix.m[2][0]),
        m21_(matrix.m[2][1]),
        m22_(matrix.m[2][2]),
        m23_(matrix.m[2][3])
    {
    }
    
    // Construct from values.
    Matrix3x4(float v00, float v01, float v02, float v03,
            float v10, float v11, float v12, float v13,
            float v20, float v21, float v22, float v23) :
        m00_(v00),
        m01_(v01),
        m02_(v02),
        m03_(v03),
        m10_(v10),
        m11_(v11),
        m12_(v12),
        m13_(v13),
        m20_(v20),
        m21_(v21),
        m22_(v22),
        m23_(v23)
    {
    }
    
    /// Construct from a float array.
    Matrix3x4(const float* data) :
        m00_(data[0]),
        m01_(data[1]),
        m02_(data[2]),
        m03_(data[3]),
        m10_(data[4]),
        m11_(data[5]),
        m12_(data[6]),
        m13_(data[7]),
        m20_(data[8]),
        m21_(data[9]),
        m22_(data[10]),
        m23_(data[11])
    {
    }
    
    /// Construct from translation, rotation and uniform scale.
    Matrix3x4(const Vector3& translation, const Quaternion& rotation, float scale);
    /// Construct from translation, rotation and nonuniform scale.
    Matrix3x4(const Vector3& translation, const Quaternion& rotation, const Vector3& scale);
    
    /// Assign from another matrix.
    Matrix3x4& operator = (const Matrix3x4& rhs)
    {
        m00_ = rhs.m00_;
        m01_ = rhs.m01_;
        m02_ = rhs.m02_;
        m03_ = rhs.m03_;
        m10_ = rhs.m10_;
        m11_ = rhs.m11_;
        m12_ = rhs.m12_;
        m13_ = rhs.m13_;
        m20_ = rhs.m20_;
        m21_ = rhs.m21_;
        m22_ = rhs.m22_;
        m23_ = rhs.m23_;
        return *this;
    }
    
    /// Assign from a 3x3 matrix and set the extra elements to identity.
    Matrix3x4& operator = (const Matrix3& rhs)
    {
        m00_ = rhs.m[0][0];
        m01_ = rhs.m[0][1];
        m02_ = rhs.m[0][2];
        m03_ = 0.0;
        m10_ = rhs.m[1][0];
        m11_ = rhs.m[1][1];
        m12_ = rhs.m[1][2];
        m13_ = 0.0;
        m20_ = rhs.m[2][0];
        m21_ = rhs.m[2][1];
        m22_ = rhs.m[2][2];
        m23_ = 0.0;
        return *this;
    }
    
    /// Assign from a 4x4 matrix which is assumed to contain no projection.
    Matrix3x4& operator = (const Matrix4& rhs)
    {
        m00_ = rhs.m[0][0];
        m01_ = rhs.m[0][1];
        m02_ = rhs.m[0][2];
        m03_ = rhs.m[0][3];
        m10_ = rhs.m[1][0];
        m11_ = rhs.m[1][1];
        m12_ = rhs.m[1][2];
        m13_ = rhs.m[1][3];
		m20_ = rhs.m[2][0];
        m21_ = rhs.m[2][1];
        m22_ = rhs.m[2][2];
		m23_ = rhs.m[2][3];
        return *this;
    }
    
    /// Test for equality with another matrix without epsilon.
    bool operator == (const Matrix3x4& rhs) const
    {
        const float* leftData = Data();
        const float* rightData = rhs.Data();
        
        for (unsigned i = 0; i < 12; ++i)
        {
            if (leftData[i] != rightData[i])
                return false;
        }
        
        return true;
    }
    
    /// Test for inequality with another matrix without epsilon.
    bool operator != (const Matrix3x4& rhs) const { return !(*this == rhs); }
    
    /// Multiply a Vector3 which is assumed to represent position.
    Vector3 operator * (const Vector3& rhs) const
    {
        return Vector3(
            (m00_ * rhs.x + m01_ * rhs.y + m02_ * rhs.z + m03_),
            (m10_ * rhs.x + m11_ * rhs.y + m12_ * rhs.z + m13_),
            (m20_ * rhs.x + m21_ * rhs.y + m22_ * rhs.z + m23_)
        );
    }
    
    /// Multiply a Vector4.
    Vector3 operator * (const Vector4& rhs) const
    {
        return Vector3(
            (m00_ * rhs.x + m01_ * rhs.y + m02_ * rhs.z + m03_ * rhs.w),
            (m10_ * rhs.x + m11_ * rhs.y + m12_ * rhs.z + m13_ * rhs.w),
            (m20_ * rhs.x + m21_ * rhs.y + m22_ * rhs.z + m23_ * rhs.w)
        );
    }
    
    /// Add a matrix.
    Matrix3x4 operator + (const Matrix3x4& rhs) const
    {
        return Matrix3x4(
            m00_ + rhs.m00_,
            m01_ + rhs.m01_,
            m02_ + rhs.m02_,
            m03_ + rhs.m03_,
            m10_ + rhs.m10_,
            m11_ + rhs.m11_,
            m12_ + rhs.m12_,
            m13_ + rhs.m13_,
            m20_ + rhs.m20_,
            m21_ + rhs.m21_,
            m22_ + rhs.m22_,
            m23_ + rhs.m23_
        );
    }
    
    /// Subtract a matrix.
    Matrix3x4 operator - (const Matrix3x4& rhs) const
    {
        return Matrix3x4(
            m00_ - rhs.m00_,
            m01_ - rhs.m01_,
            m02_ - rhs.m02_,
            m03_ - rhs.m03_,
            m10_ - rhs.m10_,
            m11_ - rhs.m11_,
            m12_ - rhs.m12_,
            m13_ - rhs.m13_,
            m20_ - rhs.m20_,
            m21_ - rhs.m21_,
            m22_ - rhs.m22_,
            m23_ - rhs.m23_
        );
    }
    
    /// Multiply with a scalar.
    Matrix3x4 operator * (float rhs) const
    {
        return Matrix3x4(
            m00_ * rhs,
            m01_ * rhs,
            m02_ * rhs,
            m03_ * rhs,
            m10_ * rhs,
            m11_ * rhs,
            m12_ * rhs,
            m13_ * rhs,
            m20_ * rhs,
            m21_ * rhs,
            m22_ * rhs,
            m23_ * rhs
        );
    }
    
    /// Multiply a matrix.
    Matrix3x4 operator * (const Matrix3x4& rhs) const
    {
        return Matrix3x4(
            m00_ * rhs.m00_ + m01_ * rhs.m10_ + m02_ * rhs.m20_,
            m00_ * rhs.m01_ + m01_ * rhs.m11_ + m02_ * rhs.m21_,
            m00_ * rhs.m02_ + m01_ * rhs.m12_ + m02_ * rhs.m22_,
            m00_ * rhs.m03_ + m01_ * rhs.m13_ + m02_ * rhs.m23_ + m03_,
            m10_ * rhs.m00_ + m11_ * rhs.m10_ + m12_ * rhs.m20_,
            m10_ * rhs.m01_ + m11_ * rhs.m11_ + m12_ * rhs.m21_,
            m10_ * rhs.m02_ + m11_ * rhs.m12_ + m12_ * rhs.m22_,
            m10_ * rhs.m03_ + m11_ * rhs.m13_ + m12_ * rhs.m23_ + m13_,
            m20_ * rhs.m00_ + m21_ * rhs.m10_ + m22_ * rhs.m20_,
            m20_ * rhs.m01_ + m21_ * rhs.m11_ + m22_ * rhs.m21_,
            m20_ * rhs.m02_ + m21_ * rhs.m12_ + m22_ * rhs.m22_,
            m20_ * rhs.m03_ + m21_ * rhs.m13_ + m22_ * rhs.m23_ + m23_
        );
    }
    
    /// Multiply a 4x4 matrix.
    Matrix4 operator * (const Matrix4& rhs) const
    {
        return Matrix4(
            m00_ * rhs.m[0][0] + m01_ * rhs.m[1][0] + m02_ * rhs.m[2][0] + m03_ * rhs.m[3][0],
            m00_ * rhs.m[0][1] + m01_ * rhs.m[1][1] + m02_ * rhs.m[2][1] + m03_ * rhs.m[3][1],
            m00_ * rhs.m[0][2] + m01_ * rhs.m[1][2] + m02_ * rhs.m[2][2] + m03_ * rhs.m[3][2],
            m00_ * rhs.m[0][3] + m01_ * rhs.m[1][3] + m02_ * rhs.m[2][3] + m03_ * rhs.m[3][3],
            m10_ * rhs.m[0][0] + m11_ * rhs.m[1][0] + m12_ * rhs.m[2][0] + m13_ * rhs.m[3][0],
            m10_ * rhs.m[0][1] + m11_ * rhs.m[1][1] + m12_ * rhs.m[2][1] + m13_ * rhs.m[3][1],
			m10_ * rhs.m[0][2] + m11_ * rhs.m[1][2] + m12_ * rhs.m[2][2] + m13_ * rhs.m[3][2],
            m10_ * rhs.m[0][3] + m11_ * rhs.m[1][3] + m12_ * rhs.m[2][3] + m13_ * rhs.m[3][3],
            m20_ * rhs.m[0][0] + m21_ * rhs.m[1][0] + m22_ * rhs.m[2][0] + m23_ * rhs.m[3][0],
            m20_ * rhs.m[0][1] + m21_ * rhs.m[1][1] + m22_ * rhs.m[2][1] + m23_ * rhs.m[3][1],
            m20_ * rhs.m[0][2] + m21_ * rhs.m[1][2] + m22_ * rhs.m[2][2] + m23_ * rhs.m[3][2],
            m20_ * rhs.m[0][3] + m21_ * rhs.m[1][3] + m22_ * rhs.m[2][3] + m23_ * rhs.m[3][3],
            rhs.m[3][0],
            rhs.m[3][1],
            rhs.m[3][2],
            rhs.m[3][3]
        );
    }
    
    /// Set translation elements.
    void SetTranslation(const Vector3& translation)
    {
        m03_ = translation.x;
        m13_ = translation.y;
        m23_ = translation.z;
    }
    
    /// Set rotation elements from a 3x3 matrix.
    void SetRotation(const Matrix3& rotation)
    {
        m00_ = rotation.m[0][0];
        m01_ = rotation.m[0][1];
        m02_ = rotation.m[0][2];
        m10_ = rotation.m[1][0];
        m11_ = rotation.m[1][1];
        m12_ = rotation.m[1][2];
		m20_ = rotation.m[2][0];
        m21_ = rotation.m[2][1];
        m22_ = rotation.m[2][2];
    }
    
    /// Set scaling elements.
    void SetScale(const Vector3& scale)
    {
        m00_ = scale.x;
        m11_ = scale.y;
        m22_ = scale.z;
    }
    
    /// Set uniform scaling elements.
    void SetScale(float scale)
    {
        m00_ = scale;
        m11_ = scale;
        m22_ = scale;
    }
    
    /// Return the combined rotation and scaling matrix.
    Matrix3 ToMatrix3() const
    {
        return Matrix3(
            m00_,
            m01_,
            m02_,
            m10_,
            m11_,
            m12_,
            m20_,
            m21_,
            m22_
        );
    }
    
    /// Convert to a 4x4 matrix by filling in an identity last row.
    Matrix4 ToMatrix4() const
    {
        return Matrix4(
            m00_,
            m01_,
            m02_,
            m03_,
            m10_,
            m11_,
            m12_,
            m13_,
            m20_,
            m21_,
            m22_,
            m23_,
            0.0f,
            0.0f,
            0.0f,
            1.0f
        );
    }
    
    /// Return the rotation matrix with scaling removed.
    Matrix3 RotationMatrix() const
    {
        Vector3 invScale(
            1.0f / sqrtf(m00_ * m00_ + m10_ * m10_ + m20_ * m20_),
            1.0f / sqrtf(m01_ * m01_ + m11_ * m11_ + m21_ * m21_),
            1.0f / sqrtf(m02_ * m02_ + m12_ * m12_ + m22_ * m22_)
        );
        
        return ToMatrix3().Scaled(invScale);
    }
    
    /// Return the translation part.
    Vector3 Translation() const
    {
        return Vector3(
            m03_,
            m13_,
            m23_
        );
    }
    
    /// Return the rotation part.
    Quaternion Rotation() const { return Quaternion(RotationMatrix()); }
    
    /// Return the scaling part.
    Vector3 Scale() const
    {
        return Vector3(
            sqrtf(m00_ * m00_ + m10_ * m10_ + m20_ * m20_),
            sqrtf(m01_ * m01_ + m11_ * m11_ + m21_ * m21_),
            sqrtf(m02_ * m02_ + m12_ * m12_ + m22_ * m22_)
        );
    }
    
    /// Test for equality with another matrix with epsilon.
    bool Equals(const Matrix3x4& rhs) const
    {
        const float* leftData = Data();
        const float* rightData = rhs.Data();
        
        for (unsigned i = 0; i < 12; ++i)
        {
            if (!Math::RealEqual(leftData[i], rightData[i]))
                return false;
        }
        
        return true;
    }
    
    /// Return decomposition to translation, rotation and scale.
    void Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const;
    /// Return inverse.
    Matrix3x4 Inverse() const;
    
    /// Return float data.
    const float* Data() const { return &m00_; }
    
    float m00_;
    float m01_;
    float m02_;
    float m03_;
    float m10_;
    float m11_;
    float m12_;
    float m13_;
    float m20_;
    float m21_;
    float m22_;
    float m23_;
    
    /// Zero matrix.
    static const Matrix3x4 ZERO;
    /// Identity matrix.
    static const Matrix3x4 IDENTITY;
};

/// Multiply a 3x4 matrix with a scalar.
inline Matrix3x4 operator * (float lhs, const Matrix3x4& rhs) { return rhs * lhs; }

/// Multiply a 3x4 matrix with a 4x4 matrix.
inline Matrix4 operator * (const Matrix4& lhs, const Matrix3x4& rhs)
{
    return Matrix4(
        lhs[0][0] * rhs.m00_ + lhs[0][1] * rhs.m10_ + lhs[0][2] * rhs.m20_,
        lhs[0][0] * rhs.m01_ + lhs[0][1] * rhs.m11_ + lhs[0][2] * rhs.m21_,
        lhs[0][0] * rhs.m02_ + lhs[0][1] * rhs.m12_ + lhs[0][2] * rhs.m22_,
        lhs[0][0] * rhs.m03_ + lhs[0][1] * rhs.m13_ + lhs[0][2] * rhs.m23_ + lhs[0][3],
        lhs[1][0] * rhs.m00_ + lhs[1][1] * rhs.m10_ + lhs[1][2] * rhs.m20_,
        lhs[1][0] * rhs.m01_ + lhs[1][1] * rhs.m11_ + lhs[1][2] * rhs.m21_,
        lhs[1][0] * rhs.m02_ + lhs[1][1] * rhs.m12_ + lhs[1][2] * rhs.m22_,
        lhs[1][0] * rhs.m03_ + lhs[1][1] * rhs.m13_ + lhs[1][2] * rhs.m23_ + lhs[1][3],
        lhs[2][0] * rhs.m00_ + lhs[2][1] * rhs.m10_ + lhs[2][2] * rhs.m20_,
        lhs[2][0] * rhs.m01_ + lhs[2][1] * rhs.m11_ + lhs[2][2] * rhs.m21_,
        lhs[2][0] * rhs.m02_ + lhs[2][1] * rhs.m12_ + lhs[2][2] * rhs.m22_,
        lhs[2][0] * rhs.m03_ + lhs[2][1] * rhs.m13_ + lhs[2][2] * rhs.m23_ + lhs[2][3],
        lhs[3][0] * rhs.m00_ + lhs[3][1] * rhs.m10_ + lhs[3][2] * rhs.m20_,
        lhs[3][0] * rhs.m01_ + lhs[3][1] * rhs.m11_ + lhs[3][2] * rhs.m21_,
        lhs[3][0] * rhs.m02_ + lhs[3][1] * rhs.m12_ + lhs[3][2] * rhs.m22_,
        lhs[3][0] * rhs.m03_ + lhs[3][1] * rhs.m13_ + lhs[3][2] * rhs.m23_ + lhs[3][3]
    );
}

}

#endif
