#include "StringUtils.h"

namespace ma 
{
	//-----------------------------------------------------------------------
	const std::string StringUtil::BLANK;
	//-----------------------------------------------------------------------
    void StringUtil::trim(std::string& str, bool left, bool right)
    {
        static const std::string delims = " \t\r";
        if(right)
            str.erase(str.find_last_not_of(delims)+1); // trim right
        if(left)
            str.erase(0, str.find_first_not_of(delims)); // trim left
    }

    //-----------------------------------------------------------------------
	std::vector<std::string> StringUtil::split( const std::string& str, const std::string& delims, unsigned int maxSplits)
    {
		std::vector<std::string> ret;
        // Pre-allocate some space for performance
        ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

        unsigned int numSplits = 0;

        // Use STL methods 
        size_t start, pos;
        start = 0;
        do 
        {
            pos = str.find_first_of(delims, start);
            if (pos == start)
            {
                // Do nothing
                start = pos + 1;
            }
            else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
            {
                // Copy the rest of the std::string
                ret.push_back( str.substr(start) );
                break;
            }
            else
            {
                // Copy up to delimiter
                ret.push_back( str.substr(start, pos - start) );
                start = pos + 1;
            }
            // parse up to next real data
            start = str.find_first_not_of(delims, start);
            ++numSplits;

        } while (pos != std::string::npos);



        return ret;
    }
	//-----------------------------------------------------------------------
	std::vector<std::string> StringUtil::tokenise( const std::string& str, const std::string& singleDelims, const std::string& doubleDelims, unsigned int maxSplits)
	{
        std::vector<std::string> ret;
        // Pre-allocate some space for performance
        ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

        unsigned int numSplits = 0;
		std::string delims = singleDelims + doubleDelims;

		// Use STL methods 
        size_t start, pos;
		char curDoubleDelim = 0;
        start = 0;
        do 
        {
			if (curDoubleDelim != 0)
			{
				pos = str.find(curDoubleDelim, start);
			}
			else
			{
				pos = str.find_first_of(delims, start);
			}

            if (pos == start)
            {
				char curDelim = str.at(pos);
				if (doubleDelims.find_first_of(curDelim) != std::string::npos)
				{
					curDoubleDelim = curDelim;
				}
                // Do nothing
                start = pos + 1;
            }
            else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
            {
				if (curDoubleDelim != 0)
				{
					//Missing closer. Warn or throw exception?
				}
                // Copy the rest of the std::string
                ret.push_back( str.substr(start) );
                break;
            }
            else
            {
				if (curDoubleDelim != 0)
				{
					curDoubleDelim = 0;
				}

				// Copy up to delimiter
				ret.push_back( str.substr(start, pos - start) );
				start = pos + 1;
            }
			if (curDoubleDelim == 0)
			{
				// parse up to next real data
				start = str.find_first_not_of(singleDelims, start);
			}
            
            ++numSplits;

        } while (pos != std::string::npos);

        return ret;
    }
    //-----------------------------------------------------------------------
    void StringUtil::toLowerCase(std::string& str)
    {
        std::transform(
            str.begin(),
            str.end(),
            str.begin(),
			(int(*)(int))tolower);
    }

    //-----------------------------------------------------------------------
    void StringUtil::toUpperCase(std::string& str) 
    {
        std::transform(
            str.begin(),
            str.end(),
            str.begin(),
			(int(*)(int))toupper);
    }
    //-----------------------------------------------------------------------
    bool StringUtil::startsWith(const std::string& str, const std::string& pattern, bool lowerCase)
    {
        size_t thisLen = str.length();
        size_t patternLen = pattern.length();
        if (thisLen < patternLen || patternLen == 0)
            return false;

        std::string startOfThis = str.substr(0, patternLen);
        if (lowerCase)
            StringUtil::toLowerCase(startOfThis);

        return (startOfThis == pattern);
    }
    //-----------------------------------------------------------------------
    bool StringUtil::endsWith(const std::string& str, const std::string& pattern, bool lowerCase)
    {
        size_t thisLen = str.length();
        size_t patternLen = pattern.length();
        if (thisLen < patternLen || patternLen == 0)
            return false;

        std::string endOfThis = str.substr(thisLen - patternLen, patternLen);
        if (lowerCase)
            StringUtil::toLowerCase(endOfThis);

        return (endOfThis == pattern);
    }
    //-----------------------------------------------------------------------
    std::string StringUtil::standardisePath(const std::string& init)
    {
        std::string path = init;

        std::replace( path.begin(), path.end(), '\\', '/' );
        if( path[path.length() - 1] != '/' )
            path += '/';

        return path;
    }
    //-----------------------------------------------------------------------
    void StringUtil::splitFilename(const std::string& qualifiedName, 
        std::string& outBasename, std::string& outPath)
    {
        std::string path = qualifiedName;
        // Replace \ with / first
        std::replace( path.begin(), path.end(), '\\', '/' );
        // split based on final /
        size_t i = path.find_last_of('/');

        if (i == std::string::npos)
        {
            outPath.clear();
			outBasename = qualifiedName;
        }
        else
        {
            outBasename = path.substr(i+1, path.size() - i - 1);
            outPath = path.substr(0, i);
        }

    }
	//-----------------------------------------------------------------------
	void StringUtil::splitBaseFilename(const std::string& fullName, 
		std::string& outBasename, std::string& outExtention)
	{
		size_t i = fullName.find_last_of(".");
		if (i == std::string::npos)
		{
			outExtention.clear();
			outBasename = fullName;
		}
		else
		{
			outExtention = fullName.substr(i+1);
			outBasename = fullName.substr(0, i);
		}
	}
	// ----------------------------------------------------------------------------------------------------------------------------------------------
	void StringUtil::splitFullFilename(	const std::string& qualifiedName, 
		std::string& outBasename, std::string& outExtention, std::string& outPath )
	{
		std::string fullName;
		splitFilename( qualifiedName, fullName, outPath );
		splitBaseFilename( fullName, outBasename, outExtention );
	}
    //-----------------------------------------------------------------------
    bool StringUtil::match(const std::string& str, const std::string& pattern, bool caseSensitive)
    {
        std::string tmpStr = str;
		std::string tmpPattern = pattern;
        if (!caseSensitive)
        {
            StringUtil::toLowerCase(tmpStr);
            StringUtil::toLowerCase(tmpPattern);
        }

        std::string::const_iterator strIt = tmpStr.begin();
        std::string::const_iterator patIt = tmpPattern.begin();
		std::string::const_iterator lastWildCardIt = tmpPattern.end();
        while (strIt != tmpStr.end() && patIt != tmpPattern.end())
        {
            if (*patIt == '*')
            {
				lastWildCardIt = patIt;
                // Skip over looking for next character
                ++patIt;
                if (patIt == tmpPattern.end())
				{
					// Skip right to the end since * matches the entire rest of the std::string
					strIt = tmpStr.end();
				}
				else
                {
					// scan until we find next pattern character
                    while(strIt != tmpStr.end() && *strIt != *patIt)
                        ++strIt;
                }
            }
            else
            {
                if (*patIt != *strIt)
                {
					if (lastWildCardIt != tmpPattern.end())
					{
						// The last wildcard can match this incorrect sequence
						// rewind pattern to wildcard and keep searching
						patIt = lastWildCardIt;
						lastWildCardIt = tmpPattern.end();
					}
					else
					{
						// no wildwards left
						return false;
					}
                }
                else
                {
                    ++patIt;
                    ++strIt;
                }
            }

        }
		// If we reached the end of both the pattern and the std::string, we succeeded
		if (patIt == tmpPattern.end() && strIt == tmpStr.end())
		{
        	return true;
		}
		else
		{
			return false;
		}

    }
	//-----------------------------------------------------------------------
	const std::string StringUtil::replaceAll(const std::string& source, const std::string& replaceWhat, const std::string& replaceWithWhat)
	{
		std::string result = source;
        std::string::size_type pos = 0;
		while(1)
		{
			pos = result.find(replaceWhat,pos);
			if (pos == std::string::npos) break;
			result.replace(pos,replaceWhat.size(),replaceWithWhat);
            pos += replaceWithWhat.size();
		}
		return result;
	}

	std::string StringUtil::getFileExt(const char* path)
	{
		const char* str = strrchr(path, '.');
		if (str == NULL)
			return "";

		std::string ext;
		size_t len = strlen(str);
		for (size_t i = 1; i < len; ++i)
			ext += tolower(str[i]);

		return ext;
	}

	std::string StringUtil::replaceFileExt(const std::string& strFile,const std::string& strNewExt)
	{
		std::string strNewFile = strFile;

		int index = strNewFile.find_last_of(".");
		if (index>=0)
		{
			strNewFile = strNewFile.replace(index + 1, strNewFile.size() - index,strNewExt);
		}

		return strNewFile;
	}

	std::string StringUtil::getDirectoryName(const char* pFullName)
	{
		std::string outBasename;
		std::string outPath;
		splitFilename(pFullName,outBasename,outPath);
		return outPath;
	}

}
