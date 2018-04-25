/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef __StringConverter_H__
#define __StringConverter_H__

#include "BaseType.h"
#include "Math/ColourValue.h"
#include "Math/Math.h"
#include "Math/Matrix3.h"
#include "Math/Matrix4.h"
#include "Math/Quaternion.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/AABB.h"

namespace ma {

	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup General
	*  @{
	*/
	/** Class for converting the core Ogre data types to/from Strings.
    @remarks
        The code for converting values to and from strings is here as a separate
        class to avoid coupling string to other datatypes (and vice-versa) which reduces
        compilation dependency: important given how often the core types are used.
    @par
        This class is mainly used for parsing settings in text files. External applications
        can also use it to interface with classes which use the StringInterface template
        class.
    @par
        The string formats of each of the major types is listed with the methods. The basic types
        like int and float just use the underlying C runtime library atof and atoi family methods,
        however custom types like Vector3, ColourValue and Matrix4 are also supported by this class
        using custom formats.
    @author
        Steve Streeting
    */
    class StringConverter
    {
    public:

        /** Converts a float to a string. */
        static string toString(float val, unsigned short precision = 6, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0) );
        /** Converts a Radian to a string. */
//         static string toString(Radian val, unsigned short precision = 6, 
//             unsigned short width = 0, char fill = ' ', 
//             std::ios::fmtflags flags = std::ios::fmtflags(0) )
//         {
//             return toString(val.valueAngleUnits(), precision, width, fill, flags);
//         }
        /** Converts a Degree to a string. */
//         static string toString(Degree val, unsigned short precision = 6, 
//             unsigned short width = 0, char fill = ' ', 
//             std::ios::fmtflags flags = std::ios::fmtflags(0) )
//         {
//             return toString(val.valueAngleUnits(), precision, width, fill, flags);
//         }
        /** Converts an int to a string. */
        static string toString(int val, unsigned short width = 0, 
            char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0) );
#if OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_64 || OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
        /** Converts an unsigned int to a string. */
        static string toString(unsigned int val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0) );
        /** Converts a size_t to a string. */
//         static string toString(size_t val, 
//             unsigned short width = 0, char fill = ' ', 
//             std::ios::fmtflags flags = std::ios::fmtflags(0) );
        #ifdef WIN32
                /** Converts an unsigned long to a string. */
                static string toString(unsigned long val, 
                    unsigned short width = 0, char fill = ' ', 
                    std::ios::fmtflags flags = std::ios::fmtflags(0) );

        #endif
#else
        /** Converts a size_t to a string. */
        static string toString(size_t val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0) );
        /** Converts an unsigned long to a string. */
        static string toString(unsigned long val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0) );
#endif
        /** Converts a long to a string. */
        static string toString(long val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0) );
        /** Converts a boolean to a string. 
        @param yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
        */
        static string toString(bool val, bool yesNo = false);
		/** Converts a Vector2 to a string. 
        @remarks
            Format is "x y" (i.e. 2x float values, space delimited)
        */
        static string toString(const Vector2& val);
        /** Converts a Vector3 to a string. 
        @remarks
            Format is "x y z" (i.e. 3x float values, space delimited)
        */
        static string toString(const Vector3& val);
		/** Converts a Vector4 to a string. 
        @remarks
            Format is "x y z w" (i.e. 4x float values, space delimited)
        */
        static string toString(const Vector4& val);
        /** Converts a Matrix3 to a string. 
        @remarks
            Format is "00 01 02 10 11 12 20 21 22" where '01' means row 0 column 1 etc.
        */
        static string toString(const Matrix3& val);
        /** Converts a Matrix4 to a string. 
        @remarks
            Format is "00 01 02 03 10 11 12 13 20 21 22 23 30 31 32 33" where 
            '01' means row 0 column 1 etc.
        */
        static string toString(const Matrix4& val);
        /** Converts a Quaternion to a string. 
        @remarks
            Format is "w x y z" (i.e. 4x float values, space delimited)
        */
        static string toString(const Quaternion& val);
        /** Converts a ColourValue to a string. 
        @remarks
            Format is "r g b a" (i.e. 4x float values, space delimited). 
        */
        static string toString(const ColourValue& val);
        /** Converts a vector<string> to a string.
        @remarks
            Strings must not contain spaces since space is used as a delimiter in
            the output.
        */
        static string toString(const vector<string>& val);
		static string toString(const AABB& val);
		static string toString(const AABB2D& val);
		static string toString(const Vec2i& val);
		static string toString(const Vec3i& val);

        /** Converts a string to a float. 
        @returns
            0.0 if the value could not be parsed, otherwise the float version of the string.
        */
        static float parseReal(const string& val, float defaultValue = 0);
        /** Converts a string to a Angle. 
        @returns
            0.0 if the value could not be parsed, otherwise the Angle version of the string.
        */
        //static inline Radian parseAngle(const string& val, Radian defaultValue = Radian(0)) {
		//	return Angle(parseReal(val, defaultValue.valueRadians()));
		//}
        /** Converts a string to a whole number. 
        @returns
            0.0 if the value could not be parsed, otherwise the numeric version of the string.
        */
        static int parseInt(const string& val, int defaultValue = 0);
        /** Converts a string to a whole number. 
        @returns
            0.0 if the value could not be parsed, otherwise the numeric version of the string.
        */
        static unsigned int parseUnsignedInt(const string& val, unsigned int defaultValue = 0);
        /** Converts a string to a whole number. 
        @returns
            0.0 if the value could not be parsed, otherwise the numeric version of the string.
        */
        static long parseLong(const string& val, long defaultValue = 0);
        /** Converts a string to a whole number. 
        @returns
            0.0 if the value could not be parsed, otherwise the numeric version of the string.
        */
        static unsigned long parseUnsignedLong(const string& val, unsigned long defaultValue = 0);
        /** Converts a string to a boolean. 
        @remarks
            Returns true if case-insensitive match of the start of the string
			matches "true", "yes" or "1", false otherwise.
        */
        static bool parseBool(const string& val, bool defaultValue = 0);
		/** Parses a Vector2 out of a string.
        @remarks
            Format is "x y" ie. 2 float components, space delimited. Failure to parse returns
            Vector2::Z
        */
		static Vector2 parseVector2(const string& val, const Vector2& defaultValue = Vector2::ZERO);
		/** Parses a Vector3 out of a string.
        @remarks
            Format is "x y z" ie. 3 float components, space delimited. Failure to parse returns
            Vector3::ZERO.
        */
        static Vector3 parseVector3(const string& val, const Vector3& defaultValue = Vector3::ZERO);
        /** Parses a Vector4 out of a string.
        @remarks
            Format is "x y z w" ie. 4 float components, space delimited. Failure to parse returns
            Vector4::ZERO.
        */
        static Vector4 parseVector4(const string& val, const Vector4& defaultValue = Vector4::ZERO);
        /** Parses a Matrix3 out of a string.
        @remarks
            Format is "00 01 02 10 11 12 20 21 22" where '01' means row 0 column 1 etc.
            Failure to parse returns Matrix3::IDENTITY.
        */
        static Matrix3 parseMatrix3(const string& val, const Matrix3& defaultValue = Matrix3::IDENTITY);
        /** Parses a Matrix4 out of a string.
        @remarks
            Format is "00 01 02 03 10 11 12 13 20 21 22 23 30 31 32 33" where 
            '01' means row 0 column 1 etc. Failure to parse returns Matrix4::IDENTITY.
        */
        static Matrix4 parseMatrix4(const string& val, const Matrix4& defaultValue = Matrix4::IDENTITY);
        /** Parses a Quaternion out of a string. 
        @remarks
            Format is "w x y z" (i.e. 4x float values, space delimited). 
            Failure to parse returns Quaternion::IDENTITY.
        */
        static Quaternion parseQuaternion(const string& val, const Quaternion& defaultValue = Quaternion::IDENTITY);
        /** Parses a ColourValue out of a string. 
        @remarks
            Format is "r g b a" (i.e. 4x float values, space delimited), or "r g b" which implies
            an alpha value of 1.0 (opaque). Failure to parse returns ColourValue::Black.
        */
        static ColourValue parseColourValue(const string& val, const ColourValue& defaultValue = ColourValue::Black);

        /** Pareses a vector<string> from a string.
        @remarks
            Strings must not contain spaces since space is used as a delimiter in
            the output.
        */
        static vector<string> parseStringVector(const string& val);

		static AABB parseAABB(const string& val, const AABB& defaultValue = AABB());
		static AABB2D parseAABB2D(const string& val, const AABB2D& defaultValue = AABB2D());
		static Vec2i parseVec2i(const string& val);
		static Vec3i parseVec3i(const string& val);

        /** Checks the string is a valid number value. */
        static bool isNumber(const string& val);
    };

	/** @} */
	/** @} */

}



#endif

