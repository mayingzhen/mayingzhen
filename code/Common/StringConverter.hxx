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

#include "StringConverter.h"
#include <sstream>
#include "StringUtils.h"

namespace ma {

    //-----------------------------------------------------------------------
    string StringConverter::toString(float val, unsigned short precision, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
        stream.precision(precision);
        stream.width(width);
        stream.fill(fill);
		stream.imbue(std::locale("C"));
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    string StringConverter::toString(int val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
		stream.width(width);
        stream.fill(fill);
		stream.imbue(std::locale("C"));
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
#if OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_64 || OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
    string StringConverter::toString(unsigned int val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
        stream.width(width);
        stream.fill(fill);
		stream.imbue(std::locale("C"));
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
//     string StringConverter::toString(size_t val, 
//         unsigned short width, char fill, std::ios::fmtflags flags)
//     {
//         stringstream stream;
//         stream.width(width);
//         stream.fill(fill);
//		   stream.imbue(std::locale("C"));
//         if (flags)
//             stream.setf(flags);
//         stream << val;
//         return stream.str();
//     }
#ifdef WIN32
    //-----------------------------------------------------------------------
    string StringConverter::toString(unsigned long val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
        stream.width(width);
        stream.fill(fill);
		stream.imbue(std::locale("C"));
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }

#endif
    //-----------------------------------------------------------------------
#else
    string StringConverter::toString(size_t val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
		stream.width(width);
        stream.fill(fill);
		stream.imbue(std::locale("C"));
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    string StringConverter::toString(unsigned long val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
		stream.width(width);
        stream.fill(fill);
		stream.imbue(std::locale("C"));
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
#endif
    string StringConverter::toString(long val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
		stream.width(width);
        stream.fill(fill);
		stream.imbue(std::locale("C"));
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
	//-----------------------------------------------------------------------
    string StringConverter::toString(const Vector2& val)
    {
        stringstream stream;
        stream.imbue(std::locale("C"));
		stream << val.x << " " << val.y;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    string StringConverter::toString(const Vector3& val)
    {
        stringstream stream;
        stream.imbue(std::locale("C"));
		stream << val.x << " " << val.y << " " << val.z;
        return stream.str();
    }
	//-----------------------------------------------------------------------
    string StringConverter::toString(const Vector4& val)
    {
        stringstream stream;
        stream.imbue(std::locale("C"));
		stream << val.x << " " << val.y << " " << val.z << " " << val.w;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    string StringConverter::toString(const Matrix3& val)
    {
		stringstream stream;
        stream.imbue(std::locale("C"));
        stream << val[0][0] << " " 
            << val[0][1] << " "             
            << val[0][2] << " "             
            << val[1][0] << " "             
            << val[1][1] << " "             
            << val[1][2] << " "             
            << val[2][0] << " "             
            << val[2][1] << " "             
            << val[2][2];
        return stream.str();
    }
    //-----------------------------------------------------------------------
    string StringConverter::toString(bool val, bool yesNo)
    {
        if (val)
        {
            if (yesNo)
            {
                return "yes";
            }
            else
            {
                return "true";
            }
        }
        else
            if (yesNo)
            {
                return "no";
            }
            else
            {
                return "false";
            }
    }
    //-----------------------------------------------------------------------
    string StringConverter::toString(const Matrix4& val)
    {
		stringstream stream;
        stream.imbue(std::locale("C"));
        stream << val[0][0] << " " 
            << val[0][1] << " "             
            << val[0][2] << " "             
            << val[0][3] << " "             
            << val[1][0] << " "             
            << val[1][1] << " "             
            << val[1][2] << " "             
            << val[1][3] << " "             
            << val[2][0] << " "             
            << val[2][1] << " "             
            << val[2][2] << " "             
            << val[2][3] << " "             
            << val[3][0] << " "             
            << val[3][1] << " "             
            << val[3][2] << " "             
            << val[3][3];
        return stream.str();
    }
    //-----------------------------------------------------------------------
    string StringConverter::toString(const Quaternion& val)
    {
		stringstream stream;
        stream.imbue(std::locale("C"));
        stream  << val.w << " " << val.x << " " << val.y << " " << val.z;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    string StringConverter::toString(const ColourValue& val)
    {
		stringstream stream;
        stream.imbue(std::locale("C"));
        stream << val.r << " " << val.g << " " << val.b << " " << val.a;
        return stream.str();
    }
	string StringConverter::toString(const AABB& val)
	{
		stringstream stream;
        stream.imbue(std::locale("C"));
		stream << val.getMinimum().x << " " << val.getMinimum().y << " " << val.getMinimum().z
			<< " " << val.getMaximum().x << " " << val.getMaximum().y << " " << val.getMaximum().z;
		return stream.str();
	}

	string StringConverter::toString(const AABB2D& val)
	{
		stringstream stream;
		stream.imbue(std::locale("C"));
		stream << val.getMinimum().x << " " << val.getMinimum().y
			<< " " << val.getMaximum().x << " " << val.getMaximum().y;
		return stream.str();
	}

	string StringConverter::toString(const Vec2i& val)
	{
		stringstream stream;
		stream.imbue(std::locale("C"));
		stream << val.x << " " << val.y;
		return stream.str();
	}
	string StringConverter::toString(const Vec3i& val)
	{
		stringstream stream;
		stream.imbue(std::locale("C"));
		stream << val.x << " " << val.y << " " << val.z;
		return stream.str();
	}
    //-----------------------------------------------------------------------
    string StringConverter::toString(const vector<string>& val)
    {
		stringstream stream;
        stream.imbue(std::locale("C"));
        vector<string>::const_iterator i, iend, ibegin;
        ibegin = val.begin();
        iend = val.end();
        for (i = ibegin; i != iend; ++i)
        {
            if (i != ibegin)
                stream << " ";

            stream << *i; 
        }
        return stream.str();
    }
    //-----------------------------------------------------------------------
    float StringConverter::parseReal(const string& val, float defaultValue)
    {
		// Use istringstream for direct correspondence with toString
// 		stringstream str(val);
// 		Real ret = defaultValue;
// 		str >> ret;

        return (float)atof(val.c_str());
    }
    //-----------------------------------------------------------------------
    int StringConverter::parseInt(const string& val, int defaultValue)
    {
		// Use istringstream for direct correspondence with toString
		stringstream str(val);
		int ret = defaultValue;
		str >> ret;

        return ret;
    }
    //-----------------------------------------------------------------------
    unsigned int StringConverter::parseUnsignedInt(const string& val, unsigned int defaultValue)
    {
		// Use istringstream for direct correspondence with toString
		stringstream str(val);
		unsigned int ret = defaultValue;
		str >> ret;

		return ret;
    }
    //-----------------------------------------------------------------------
    long StringConverter::parseLong(const string& val, long defaultValue)
    {
		// Use istringstream for direct correspondence with toString
		stringstream str(val);
		long ret = defaultValue;
		str >> ret;

		return ret;
    }
    //-----------------------------------------------------------------------
    unsigned long StringConverter::parseUnsignedLong(const string& val, unsigned long defaultValue)
    {
		// Use istringstream for direct correspondence with toString
		stringstream str(val);
		unsigned long ret = defaultValue;
		str >> ret;

		return ret;
    }
    //-----------------------------------------------------------------------
    bool StringConverter::parseBool(const string& val, bool defaultValue)
    {
		if ((StringUtil::startsWith(val, "true") || StringUtil::startsWith(val, "yes")
			|| StringUtil::startsWith(val, "1")))
			return true;
		else if ((StringUtil::startsWith(val, "false") || StringUtil::startsWith(val, "no")
			|| StringUtil::startsWith(val, "0")))
			return false;
		else
			return defaultValue;
    }
    //-----------------------------------------------------------------------
    Vector2 StringConverter::parseVector2(const string& val, const Vector2& defaultValue)
    {
        // Split on space
        vector<string> vec = StringUtil::split(val);

        if (vec.size() != 2)
        {
            return defaultValue;
        }
        else
        {
            return Vector2(parseReal(vec[0]),parseReal(vec[1]));
        }
    }
	//-----------------------------------------------------------------------
    Vector3 StringConverter::parseVector3(const string& val, const Vector3& defaultValue)
    {
        // Split on space
        vector<string> vec = StringUtil::split(val);

        if (vec.size() != 3)
        {
            return defaultValue;
        }
        else
        {
            return Vector3(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]));
        }
    }
	//-----------------------------------------------------------------------
    Vector4 StringConverter::parseVector4(const string& val, const Vector4& defaultValue)
    {
        // Split on space
        vector<string> vec = StringUtil::split(val);

        if (vec.size() != 4)
        {
            return defaultValue;
        }
        else
        {
            return Vector4(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]),parseReal(vec[3]));
        }
    }
    //-----------------------------------------------------------------------
    Matrix3 StringConverter::parseMatrix3(const string& val, const Matrix3& defaultValue)
    {
        // Split on space
        vector<string> vec = StringUtil::split(val);

        if (vec.size() != 9)
        {
            return defaultValue;
        }
        else
        {
            return Matrix3(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]),
                parseReal(vec[3]),parseReal(vec[4]),parseReal(vec[5]),
                parseReal(vec[6]),parseReal(vec[7]),parseReal(vec[8]));
        }
    }
    //-----------------------------------------------------------------------
    Matrix4 StringConverter::parseMatrix4(const string& val, const Matrix4& defaultValue)
    {
        // Split on space
        vector<string> vec = StringUtil::split(val);

        if (vec.size() != 16)
        {
            return defaultValue;
        }
        else
        {
            return Matrix4(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]), parseReal(vec[3]),
                parseReal(vec[4]),parseReal(vec[5]), parseReal(vec[6]), parseReal(vec[7]),
                parseReal(vec[8]),parseReal(vec[9]), parseReal(vec[10]), parseReal(vec[11]),
                parseReal(vec[12]),parseReal(vec[13]), parseReal(vec[14]), parseReal(vec[15]));
        }
    }
    //-----------------------------------------------------------------------
    Quaternion StringConverter::parseQuaternion(const string& val, const Quaternion& defaultValue)
    {
        // Split on space
        vector<string> vec = StringUtil::split(val);

        if (vec.size() != 4)
        {
            return defaultValue;
        }
        else
        {
            return Quaternion(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]), parseReal(vec[3]));
        }
    }
    //-----------------------------------------------------------------------
    ColourValue StringConverter::parseColourValue(const string& val, const ColourValue& defaultValue)
    {
        // Split on space
        vector<string> vec = StringUtil::split(val);

        if (vec.size() == 4)
        {
            return ColourValue(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]), parseReal(vec[3]));
        }
        else if (vec.size() == 3)
        {
            return ColourValue(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]), 1.0f);
        }
        else
        {
            return defaultValue;
        }
    }
    //-----------------------------------------------------------------------
    vector<string> StringConverter::parseStringVector(const string& val)
    {
        return StringUtil::split(val);
    }
	//-----------------------------------------------------------------------
	bool StringConverter::isNumber(const string& val)
	{
		stringstream str(val);
		float tst;
		str >> tst;
		return !str.fail() && str.eof();
	}

	AABB StringConverter::parseAABB( const string& val, const AABB& defaultValue /*= AABB()*/ )
	{
		// Split on space
		vector<string> vec = StringUtil::split(val);
		if (vec.size() == 6)
		{
			return AABB(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]), parseReal(vec[3]), parseReal(vec[4]), parseReal(vec[5]));
		}
	
		return defaultValue;
	}	

	AABB2D StringConverter::parseAABB2D(const string& val, const AABB2D& defaultValue/* = AABB2D()*/)
	{
		// Split on space
		vector<string> vec = StringUtil::split(val);
		if (vec.size() == 4)
		{
			return AABB2D(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]), parseReal(vec[3]));
		}

		return defaultValue;
	}

	Vec2i StringConverter::parseVec2i(const string& val)
	{
		vector<string> vec = StringUtil::split(val);
		if (vec.size() == 2)
		{
			return Vec2i(parseInt(vec[0]), parseInt(vec[1]));
		}

		return Vec2i(0, 0);
	}

	Vec3i StringConverter::parseVec3i(const string& val)
	{
		vector<string> vec = StringUtil::split(val);
		if (vec.size() == 3)
		{
			return Vec3i(parseInt(vec[0]), parseInt(vec[1]), parseInt(vec[2]));
		}

		return Vec3i(0, 0, 0);
	}
}


