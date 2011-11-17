#ifndef __XML_FILE_H__
#define __XML_FILE_H__

#include <string>
#include <list>
#include <d3dx9math.h>


using std::string;
using std::list;

typedef D3DXVECTOR4 float4;
typedef D3DXVECTOR3 float3;
typedef D3DXVECTOR2 float2;


///////////////////////////////////////////////////////////////////////////////
enum XmlParserError
{
	XPE_Unknown	= 0,
	XPE_OpenFile,			//无法打开文件
	XPE_ReadFile,			//无法读取文件
	XPE_BeginEndMismatch,	//节点起始/结束不匹配
	XPE_XmlDefine,			//xml定义块不正确
	XPE_Comment,			//注释块不正确
	XPE_NodeName,			//节点名不合法
	XPE_NodeNameMismatch,	//前后节点名不匹配
};

///////////////////////////////////////////////////////////////////////////////
//CXmlAttribute类
///////////////////////////////////////////////////////////////////////////////
class CXmlAttribute
{
	friend class CXmlNode;
	friend class CXmlFile;

public:
	void SetValue( const char* pValue );
	inline const char* GetValue();
	inline const char* GetName();

private:
	string	m_strName;
	string	m_strValue;
};

///////////////////////////////////////////////////////////////////////////////
//CXmlNode类
///////////////////////////////////////////////////////////////////////////////
class CXmlNode
{
	friend class CXmlFile;
	friend class CXmlStruct;

public:
	CXmlNode();
	~CXmlNode();

	typedef list<CXmlNode*>::iterator INDEX;
	typedef list<CXmlAttribute*>::iterator INDEX_ATTRIBUTE;

public:
	//////////////////////////////////////////////////////////////////////////////
	inline CXmlNode* GetParent();

	inline bool IsEmpty();

	inline bool HasChild();

	inline CXmlNode* GetFirstChild( INDEX& index );
	inline CXmlNode* GetNextChild( INDEX& index );
	inline CXmlNode* GetChild( INDEX index );

	CXmlNode* FindChild( const char* pChildName );
	CXmlNode* FindFirstChild( const char* pChildName, INDEX& index );
	CXmlNode* FindNextChild( const char* pChildName, INDEX& index );

	inline CXmlAttribute* GetFirstAttribute( INDEX_ATTRIBUTE& index );
	inline CXmlAttribute* GetNextAttribute( INDEX_ATTRIBUTE& index );
	inline CXmlAttribute* GetAttribute( INDEX_ATTRIBUTE index );

	void RemoveChild( INDEX index );

	void ClearChild();

	CXmlNode* AppendChild( const char* pChildName );

	CXmlAttribute* AppendAttribute( const char* pAttrName );

	CXmlAttribute* FindAttribute( const char* pAttrName );

	void ClearAttributes();

	//////////////////////////////////////////////////////////////////////////////
	inline const char* GetName();

	inline const char*	GetValue_string();
	inline bool			GetValue_bool();
	inline int			GetValue_int();
	inline unsigned	int	GetValue_uint();
	inline float		GetValue_float();
	inline float2		GetValue_float2();
	inline float3		GetValue_float3();
	inline float4		GetValue_float4();

	inline void SetValue_string( const string& strValue );
	inline void SetValue_bool( bool bValue );
	void SetValue_int( int iValue );
	void SetValue_float( float fValue );
	void SetValue_float2( const float2& f2Value );
	void SetValue_float3( const float3& f3Value );
	void SetValue_float4( const float4& f4Value );

protected:
	CXmlNode* AddChild();

	void SaveNode( FILE* fp, int iDepth );

	void SaveChildNode( FILE* fp, int iDepth );

	inline void SetEmpty( bool bEmpty );

private:
	string		m_strName;
	string		m_strContent;

	list<CXmlAttribute*>	m_listAttribute;

	list<CXmlNode*>	m_listChild;
	CXmlNode*		m_pParent;

	bool			m_bEmpty;
};

///////////////////////////////////////////////////////////////////////////////
//CXmlFile类
///////////////////////////////////////////////////////////////////////////////
class CXmlFile : public CXmlNode
{
public:
	CXmlFile();

public:
	bool LoadFromFile( const char* pFilename );
	bool LoadFromMemory( const char* pBuffer, size_t dwBufferLen );

	bool Save( const char* pFilename );

	void Reset();

	inline XmlParserError GetLastError();

private:
	bool Parse( const char* pInput, size_t dwSize );

	bool FindLabel( const char* &pBegin, const char* &pLabel, size_t &dwLabelLen );

	void ParseLabel( CXmlNode* pNode, const char* pLabel, size_t dwSizeLabel );

	inline void SetError( XmlParserError error );

private:
	XmlParserError	m_LastError;
};

///////////////////////////////////////////////////////////////////////////////
inline const char* CXmlAttribute::GetValue()
{
	return m_strValue.c_str();
}

///////////////////////////////////////////////////////////////////////////////
inline const char* CXmlAttribute::GetName()
{
	return m_strName.c_str();
}

///////////////////////////////////////////////////////////////////////////////
inline bool CXmlNode::IsEmpty()
{
	return m_bEmpty;
}

///////////////////////////////////////////////////////////////////////////////
inline bool CXmlNode::HasChild()
{
	return ( !m_listChild.empty() );
}

///////////////////////////////////////////////////////////////////////////////
inline CXmlNode* CXmlNode::GetParent()
{
	return m_pParent;
}

///////////////////////////////////////////////////////////////////////////////
inline CXmlNode* CXmlNode::GetFirstChild( INDEX& index )
{
	index = m_listChild.begin();
	if ( index != m_listChild.end() )
	{
		return *index;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
inline CXmlNode* CXmlNode::GetNextChild( INDEX& index )
{
	if ( index != m_listChild.end() )
	{
		++index;
		if ( index != m_listChild.end() )
		{
			return *index;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
inline CXmlNode* CXmlNode::GetChild( INDEX index )
{
	if ( index != m_listChild.end() )
	{
		return *index;
	}
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
inline CXmlAttribute* CXmlNode::GetFirstAttribute( INDEX_ATTRIBUTE& index )
{
	index = m_listAttribute.begin();
	if ( index != m_listAttribute.end() )
	{
		return *index;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
inline CXmlAttribute* CXmlNode::GetNextAttribute( INDEX_ATTRIBUTE& index )
{
	if ( index != m_listAttribute.end() )
	{
		++index;
		if ( index != m_listAttribute.end() )
		{
			return *index;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
inline CXmlAttribute* CXmlNode::GetAttribute( INDEX_ATTRIBUTE index )
{
	if ( index != m_listAttribute.end() )
	{
		return *index;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
inline const char* CXmlNode::GetName()
{
	return m_strName.c_str();
}

///////////////////////////////////////////////////////////////////////////////
inline const char* CXmlNode::GetValue_string()
{
	return m_strContent.c_str();
}

///////////////////////////////////////////////////////////////////////////////
inline bool CXmlNode::GetValue_bool()
{
	// liujing 为了兼容"0,1"做出修改
	// return ( m_strContent == "true" );
	return (( m_strContent != "0" ) && ( m_strContent !="false" ));
}

///////////////////////////////////////////////////////////////////////////////
inline int CXmlNode::GetValue_int()
{
	return atoi( m_strContent.c_str() );
}
inline unsigned	int CXmlNode::GetValue_uint()
{
	unsigned int uValue;
	uValue = (unsigned int)(atol(m_strContent.c_str()));
	return uValue;
}
///////////////////////////////////////////////////////////////////////////////
inline float CXmlNode::GetValue_float()
{
	return static_cast<float>( atof( m_strContent.c_str() ) );
}

///////////////////////////////////////////////////////////////////////////////
inline void CXmlNode::SetValue_string( const string& strValue )
{
	m_strContent = strValue;
}

///////////////////////////////////////////////////////////////////////////////
inline void CXmlNode::SetValue_bool( bool bValue )
{
	m_strContent = bValue ? "true" : "false";
}

///////////////////////////////////////////////////////////////////////////////
inline float2 CXmlNode::GetValue_float2()
{
	float2 vec2;
	const char *pContent = m_strContent.c_str();
	if (*pContent == '(') pContent++;
	sscanf(pContent,"%f,%f",&vec2.x,&vec2.y);
	return vec2;
}

///////////////////////////////////////////////////////////////////////////////
inline float3 CXmlNode::GetValue_float3()
{
	float3 vec3;
	const char *pContent = m_strContent.c_str();
	if (*pContent == '(') pContent++;
	sscanf(pContent,"%f,%f,%f",&vec3.x,&vec3.y,&vec3.z);
	return vec3;
}

///////////////////////////////////////////////////////////////////////////////
inline float4 CXmlNode::GetValue_float4()
{
	float4 vec4;
	const char *pContent = m_strContent.c_str();
	if (*pContent == '(') pContent++;
	sscanf(pContent,"%f,%f,%f,%f",&vec4.x,&vec4.y,&vec4.z,&vec4.w);
	return vec4;
}

///////////////////////////////////////////////////////////////////////////////
inline void CXmlNode::SetEmpty( bool bEmpty )
{
	m_bEmpty = bEmpty;
}

///////////////////////////////////////////////////////////////////////////////
inline XmlParserError CXmlFile::GetLastError()
{
	return m_LastError;
}

///////////////////////////////////////////////////////////////////////////////
inline void CXmlFile::SetError( XmlParserError error )
{
	m_LastError = error;
}

#endif
