#include "StringHash.h"

#include <cstdio>


namespace ma
{

const StringHash StringHash::ZERO;

inline unsigned SDBMHash(unsigned hash, unsigned char c) { return c + (hash << 6) + (hash << 16) - hash; }

StringHash::StringHash(const char* str) :
    m_nValue(Calculate(str))
{
	m_strDebug = str ? str : "";
}

StringHash::StringHash(const string& str) :
    m_nValue(Calculate(str.c_str()))
{
	m_strDebug = str;
}

unsigned StringHash::Calculate(const char* str)
{
    unsigned hash = 0;

    if (!str)
        return hash;

    while (*str)
    {
        // Perform the actual hashing as case-insensitive
        char c = *str;
        hash = SDBMHash(hash, (unsigned char)tolower(c));
        ++str;
    }

    return hash;
}

// string StringHash::ToString() const
// {
//     char tempBuffer[CONVERSION_BUFFER_LENGTH];
//     sprintf(tempBuffer, "%08X", m_nValue);
//     return String(tempBuffer);
// }

}
