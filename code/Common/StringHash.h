#pragma once

namespace ma
{

/// 32-bit hash value for a string.
class StringHash
{
public:
    /// Construct with zero value.
    StringHash() :
        m_nValue(0)
    {
    }

    /// Copy-construct from another hash.
    StringHash(const StringHash& rhs) :
        m_nValue(rhs.m_nValue)
    {
    }

    /// Construct with an initial value.
    explicit StringHash(unsigned value) :
        m_nValue(value)
    {
    }

    /// Construct from a C string case-insensitively.
    StringHash(const char* str);
    /// Construct from a string case-insensitively.
    StringHash(const string& str);

    /// Assign from another hash.
    StringHash& operator =(const StringHash& rhs)
    {
        m_nValue = rhs.m_nValue;
        return *this;
    }

    /// Add a hash.
    StringHash operator +(const StringHash& rhs) const
    {
        StringHash ret;
        ret.m_nValue = m_nValue + rhs.m_nValue;
        return ret;
    }

    /// Add-assign a hash.
    StringHash& operator +=(const StringHash& rhs)
    {
        m_nValue += rhs.m_nValue;
        return *this;
    }

    /// Test for equality with another hash.
    bool operator ==(const StringHash& rhs) const { return m_nValue == rhs.m_nValue; }

    /// Test for inequality with another hash.
    bool operator !=(const StringHash& rhs) const { return m_nValue != rhs.m_nValue; }

    /// Test if less than another hash.
    bool operator <(const StringHash& rhs) const { return m_nValue < rhs.m_nValue; }

    /// Test if greater than another hash.
    bool operator >(const StringHash& rhs) const { return m_nValue > rhs.m_nValue; }

    /// Return true if nonzero hash value.
    operator bool() const { return m_nValue != 0; }

    /// Return hash value.
    unsigned Value() const { return m_nValue; }

    /// Return as string.
    //string ToString() const;

    /// Return hash value for HashSet & HashMap.
    unsigned ToHash() const { return m_nValue; }

    /// Calculate hash value case-insensitively from a C string.
    static unsigned Calculate(const char* str);

    /// Zero hash.
    static const StringHash ZERO;

private:
    /// Hash value.
    unsigned m_nValue;

	string m_strDebug;
};

}
