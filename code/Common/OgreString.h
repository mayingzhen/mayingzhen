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
#ifndef _String_H__
#define _String_H__

//#include "OgrePrerequisites.h"
#include "BaseCode.h"

namespace Ogre {
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup General
	*  @{
	*/

    /** Utility class for manipulating Strings.  */
    class StringUtil
    {
	public:
		//typedef StringStream StrStreamType;

        /** Removes any whitespace characters, be it standard space or
            TABs and so on.
            @remarks
                The user may specify wether they want to trim only the
                beginning or the end of the string ( the default action is
                to trim both).
        */
        static void trim( string& str, bool left = true, bool right = true );

        /** Returns a StringVector that contains all the substrings delimited
            by the characters in the passed <code>delims</code> argument.
            @param
                delims A list of delimiter characters to split by
            @param
                maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
                parameters is > 0, the splitting process will stop after this many splits, left to right.
        */
		static vector<string> split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0);

		/** Returns a StringVector that contains all the substrings delimited
            by the characters in the passed <code>delims</code> argument, 
			or in the <code>doubleDelims</code> argument, which is used to include (normal) 
			delimeters in the tokenised string. For example, "strings like this".
            @param
                delims A list of delimiter characters to split by
			@param
                delims A list of double delimeters characters to tokenise by
            @param
                maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
                parameters is > 0, the splitting process will stop after this many splits, left to right.
        */
		static vector<string> tokenise( const std::string& str, const std::string& delims = "\t\n ", const std::string& doubleDelims = "\"", unsigned int maxSplits = 0);

		/** Lower-cases all the characters in the string.
        */
        static void toLowerCase( string& str );

        /** Upper-cases all the characters in the string.
        */
        static void toUpperCase( string& str );


        /** Returns whether the string begins with the pattern passed in.
        @param pattern The pattern to compare with.
        @param lowerCase If true, the start of the string will be lower cased before
            comparison, pattern should also be in lower case.
        */
        static bool startsWith(const std::string& str, const std::string& pattern, bool lowerCase = true);

        /** Returns whether the string ends with the pattern passed in.
        @param pattern The pattern to compare with.
        @param lowerCase If true, the end of the string will be lower cased before
            comparison, pattern should also be in lower case.
        */
        static bool endsWith(const std::string& str, const std::string& pattern, bool lowerCase = true);

        /** Method for standardising paths - use forward slashes only, end with slash.
        */
        static string standardisePath( const string &init);

        /** Method for splitting a fully qualified filename into the base name
            and path.
        @remarks
            Path is standardised as in standardisePath
        */
        static void splitFilename(const std::string& qualifiedName,
            string& outBasename, string& outPath);

		/** Method for splitting a fully qualified filename into the base name,
		extension and path.
		@remarks
		Path is standardised as in standardisePath
		*/
		static void splitFullFilename(const std::string& qualifiedName, 
			string& outBasename, string& outExtention, 
			string& outPath);

		/** Method for splitting a filename into the base name
		and extension.
		*/
		static void splitBaseFilename(const std::string& fullName, 
			string& outBasename, string& outExtention);


        /** Simple pattern-matching routine allowing a wildcard pattern.
        @param str string to test
        @param pattern Pattern to match against; can include simple '*' wildcards
        @param caseSensitive Whether the match is case sensitive or not
        */
        static bool match(const std::string& str, const std::string& pattern, bool caseSensitive = true);


		/** replace all instances of a sub-string with a another sub-string.
		@param source Source string
		@param replaceWhat Sub-string to find and replace
		@param replaceWithWhat Sub-string to replace with (the new sub-string)
		@returns An updated string with the sub-string replaced
		*/
		static const string replaceAll(const std::string& source, const std::string& replaceWhat, const std::string& replaceWithWhat);

        /// Constant blank string, useful for returning by ref where local does not exist
        static const string BLANK;
    };


// #if OGRE_COMPILER == OGRE_COMPILER_GNUC && OGRE_COMP_VER >= 310 && !defined(STLPORT)
// #   if OGRE_COMP_VER < 430
// 	typedef ::__gnu_cxx::hash< _StringBase > _StringHash;
// #   else
// 	typedef ::std::tr1::hash< _StringBase > _StringHash;
// #   endif
// #elif OGRE_COMPILER == OGRE_COMPILER_MSVC && OGRE_COMP_VER >= 1600 && !defined(STLPORT) // VC++ 10.0
// 	typedef ::std::tr1::hash< _StringBase > _StringHash;
// #elif !defined( _STLP_HASH_FUN_H )
// 	typedef stdext::hash_compare< _StringBase, std::less< _StringBase > > _StringHash;
// #else
// 	typedef std::hash< _StringBase > _StringHash;
// #endif 
	/** @} */
	/** @} */

} // namespace Ogre

#endif // _String_H__
