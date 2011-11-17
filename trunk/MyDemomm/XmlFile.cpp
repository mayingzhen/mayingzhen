///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include <Windows.h>

#include <io.h>
#include <assert.h>
#include <algorithm>
#include <strsafe.h>



#include "XmlFile.h"


const char* XML_DEFINE_BEGIN = "?xml";
const char* XML_DEFINE_END = "?";
const char* XML_COMMENT_BEGIN = "!--";
const char* XML_COMMENT_END = "--";

///////////////////////////////////////////////////////////////////////////////
void CXmlAttribute::SetValue( const char* pValue )
{
	assert( pValue != NULL );
	m_strValue = pValue;
}

///////////////////////////////////////////////////////////////////////////////
//构造
///////////////////////////////////////////////////////////////////////////////
CXmlNode::CXmlNode()
	: m_pParent( NULL )
	, m_bEmpty( false )
{
}

///////////////////////////////////////////////////////////////////////////////
//析构
///////////////////////////////////////////////////////////////////////////////
CXmlNode::~CXmlNode()
{
	ClearChild();
	ClearAttributes();
}

///////////////////////////////////////////////////////////////////////////////
void CXmlNode::SetValue_int( int iValue )
{
	char sz[128];
	::StringCchPrintfA( sz, 128, "%d", iValue );
	m_strContent = sz;
}


///////////////////////////////////////////////////////////////////////////////
void CXmlNode::SetValue_float( float fValue )
{
	char sz[128];
	::StringCchPrintfA( sz, 128, "%g", fValue );
	m_strContent = sz;
}

///////////////////////////////////////////////////////////////////////////////
void CXmlNode::SetValue_float2( const float2& f2Value )
{
	char sz[128];
	::StringCchPrintfA( sz, 128, "%g,%g", f2Value.x, f2Value.y );
	m_strContent = sz;
}

///////////////////////////////////////////////////////////////////////////////
void CXmlNode::SetValue_float3( const float3& f3Value )
{
	char sz[128];
	::StringCchPrintfA( sz, 128, "%g,%g,%g", f3Value.x, f3Value.y, f3Value.z );
	m_strContent = sz;
}

///////////////////////////////////////////////////////////////////////////////
void CXmlNode::SetValue_float4( const float4& f4Value )
{
	char sz[128];
	::StringCchPrintfA( sz, 128, "%g,%g,%g,%g", f4Value.x, f4Value.y, f4Value.z, f4Value.w );
	m_strContent = sz;
}

///////////////////////////////////////////////////////////////////////////////
CXmlNode* CXmlNode::FindChild( const char* pChildName )
{
	assert( pChildName != NULL );

	for ( list<CXmlNode*>::iterator iter = m_listChild.begin();
		  iter != m_listChild.end();
		  ++iter )
	{
		CXmlNode* pChild = *iter;
		assert( pChild != NULL );
		if ( pChild->m_strName == pChildName )
		{
			return pChild;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
CXmlNode* CXmlNode::FindFirstChild( const char* pChildName, INDEX& index )
{
	assert( pChildName != NULL );

	index = m_listChild.begin();
	while ( index != m_listChild.end() )
	{
		CXmlNode* pChild = *index;
		assert( pChild != NULL );
		if ( pChild->m_strName == pChildName )
		{
			return pChild;
		}
		++index;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
CXmlNode* CXmlNode::FindNextChild( const char* pChildName, INDEX& index )
{
	assert( pChildName != NULL );

	if ( index != m_listChild.end() )
	{
		while ( ++index != m_listChild.end() )
		{
			CXmlNode* pChild = *index;
			assert( pChild != NULL );
			if ( pChild->m_strName == pChildName )
			{
				return pChild;
			}
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
void CXmlNode::RemoveChild( INDEX index )
{
	if ( index != m_listChild.end() )
	{
		CXmlNode* pNode = *index;
		assert( pNode != NULL );
		delete pNode;
		m_listChild.erase( index );
	}
}

///////////////////////////////////////////////////////////////////////////////
void CXmlNode::ClearChild()
{
	for ( list<CXmlNode*>::iterator iter = m_listChild.begin();
		  iter != m_listChild.end();
		  ++iter )
	{
		CXmlNode* pChild = *iter;
		assert( pChild != NULL );
		delete pChild;
	}
	m_listChild.clear();
}

///////////////////////////////////////////////////////////////////////////////
CXmlNode* CXmlNode::AppendChild( const char* pChildName )
{
	CXmlNode* pChild = new CXmlNode;
	pChild->m_strName = pChildName;

	pChild->m_pParent = this;
	m_listChild.push_back( pChild );
	
	return pChild;
}

///////////////////////////////////////////////////////////////////////////////
CXmlAttribute* CXmlNode::AppendAttribute( const char* pAttrName )
{
	CXmlAttribute* pAttribute = new CXmlAttribute;
	pAttribute->m_strName = pAttrName;

	m_listAttribute.push_back( pAttribute );

	return pAttribute;
}

///////////////////////////////////////////////////////////////////////////////
CXmlAttribute* CXmlNode::FindAttribute( const char* pAttrName )
{
	for ( list<CXmlAttribute*>::iterator iter = m_listAttribute.begin();
		  iter != m_listAttribute.end();
		  ++iter )
	{
		CXmlAttribute* pAttribute = *iter;
		assert( pAttribute != NULL );
		if ( pAttribute->m_strName == pAttrName )
		{
			return pAttribute;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
void CXmlNode::ClearAttributes()
{
	for ( list<CXmlAttribute*>::iterator iter = m_listAttribute.begin();
		  iter != m_listAttribute.end();
		  ++iter )
	{
		CXmlAttribute *pAttribute = *iter;
		assert( pAttribute != NULL );
		delete pAttribute;
	}
	m_listAttribute.clear();
}

///////////////////////////////////////////////////////////////////////////////
//添加子节点
///////////////////////////////////////////////////////////////////////////////
CXmlNode* CXmlNode::AddChild()
{
	CXmlNode* pChild = new CXmlNode;

	pChild->m_pParent = this;
	m_listChild.push_back( pChild );

	return pChild;
}

///////////////////////////////////////////////////////////////////////////////
//保存节点（及其子节点）
//方便起见，这里不考虑写文件失败的情况
///////////////////////////////////////////////////////////////////////////////
void CXmlNode::SaveNode( FILE* fp, int iDepth )
{
	assert( fp != NULL );

	//开始标签
	//根据深度添加tab符
	int iTabCount = iDepth;
	while ( iTabCount-- > 0 )
	{
		fputc( '	', fp );
	}
	fputc( '<', fp );
	fputs( m_strName.c_str(), fp );

	//属性
	for ( list<CXmlAttribute*>::iterator iter = m_listAttribute.begin();
		  iter != m_listAttribute.end();
		  ++iter )
	{
		CXmlAttribute* pAttribute = *iter;
		assert( pAttribute != NULL );
		fputc( ' ', fp );
		fputs( pAttribute->m_strName.c_str(), fp );
		fputc( '=', fp );
		fputc( '"', fp );
		fputs( pAttribute->m_strValue.c_str(), fp );
		fputc( '"', fp );
	}

	// 处理 Label 结束处的字符
	if (iDepth < 0)
	{
		// XML 定义块，Label 需要以“?”结束
		assert(strcmp(m_strName.c_str(), XML_DEFINE_BEGIN) == 0);
		fputs(XML_DEFINE_END, fp);
	}
	else if ( m_bEmpty )
	{
		//空节点（包括注释）
		if ( strncmp( m_strName.c_str(), XML_COMMENT_BEGIN, strlen(XML_COMMENT_BEGIN) ) != 0 )
		{
			fputc( '/', fp );
		}
		fputc( '>', fp );
		fputs( "\r\n", fp );
		return;
	}
	fputc( '>', fp );

	if ( !HasChild() )
	{
		//叶节点，所有内容写在同一行
		fputs( m_strContent.c_str(), fp );
		fputs( "</", fp );
		fputs( m_strName.c_str(), fp );
		fputc( '>', fp );
		fputs( "\r\n", fp );
		return;
	}
	fputs( "\r\n", fp );
	//子节点
	SaveChildNode( fp, iDepth );

	// XML 定义块不需要结束标签
	if (iDepth < 0)
		return;

	//结束标签
	//根据深度添加tab符
	iTabCount = iDepth;
	while ( iTabCount-- > 0 )
	{
		fputc( '	', fp );
	}
	fputs( "</", fp );
	fputs( m_strName.c_str(), fp );
	fputc( '>', fp );
	fputs( "\r\n", fp );
}

///////////////////////////////////////////////////////////////////////////////
//保存字节点
///////////////////////////////////////////////////////////////////////////////
void CXmlNode::SaveChildNode( FILE* fp, int iDepth )
{
	for ( list<CXmlNode*>::iterator iter = m_listChild.begin();
		  iter != m_listChild.end();
		  ++iter )
	{
		CXmlNode* pChild = *iter;
		assert( pChild != NULL );
		pChild->SaveNode( fp, iDepth + 1 );
	}
}

///////////////////////////////////////////////////////////////////////////////
//class CXmlFile
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//构造
///////////////////////////////////////////////////////////////////////////////
CXmlFile::CXmlFile()
	: m_LastError( XPE_Unknown )
{
	Reset();
}

///////////////////////////////////////////////////////////////////////////////
//从文件中加载
///////////////////////////////////////////////////////////////////////////////
bool CXmlFile::LoadFromFile( const char* pFilename )
{
	assert( pFilename != NULL );

	FILE* fp = fopen( pFilename, "rb" );
	if ( NULL == fp )
	{
		return false;
	}
	size_t dwSize = _filelength( _fileno( fp ) );
	
	//TJ3_NEW
	char* pBuffer = new char[dwSize + 1];	//\0

	if ( fread( pBuffer, dwSize, 1, fp ) != 1 )
	{
		delete[] pBuffer;
		fclose( fp );
		return false;
	}
	fclose( fp );

	pBuffer[dwSize] = 0;
	assert( dwSize == strlen( pBuffer ) );

	bool bSucceed = LoadFromMemory( pBuffer, dwSize );

	//TJ3_DELETE
	delete[] pBuffer;

	return bSucceed;
}

///////////////////////////////////////////////////////////////////////////////
//从内存中加载
///////////////////////////////////////////////////////////////////////////////
bool CXmlFile::LoadFromMemory( const char* pBuffer, size_t dwBufferLen )
{
	Reset();

	return Parse( pBuffer, dwBufferLen );
}

///////////////////////////////////////////////////////////////////////////////
//保存到文件
///////////////////////////////////////////////////////////////////////////////
bool CXmlFile::Save( const char* pFilename )
{
	assert( pFilename != NULL );

	FILE* fp = fopen( pFilename, "wb" );
	if ( NULL == fp )
	{
		return false;
	}

	SaveNode( fp, -1 );

	fclose( fp );

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// 重置包括定义块在内的所有 XML 结构
//
///////////////////////////////////////////////////////////////////////////////
void CXmlFile::Reset()
{
	// 删除所有子节点和属性
	ClearChild();
	ClearAttributes();

	// 构造定义块
	m_strName = XML_DEFINE_BEGIN;
	CXmlAttribute *pAttr = AppendAttribute("version");
	assert(pAttr != NULL);
	pAttr->SetValue("1.0");
	pAttr = AppendAttribute("encoding");
	assert(pAttr != NULL);
	pAttr->SetValue("GB2312");

	// CXmlFile 是一个特殊的 CXmlNode，其对象本身用来描述 XML 定义块，同时又是
	// 非空节点
}

///////////////////////////////////////////////////////////////////////////////
//解析xml
//pInput	[in]	待解析xml字符串
//dwSize	[in]	待解析xml字符串长度
//return			是否成功
///////////////////////////////////////////////////////////////////////////////
bool CXmlFile::Parse( const char* pInput, size_t dwSize )
{
	assert( pInput != NULL );

	const char* pCur = pInput;
	const char* pEnd = pInput + dwSize;

	const char* pLabel = NULL;
	size_t dwSizeLabel = 0;

	//当前解析树深度
	int iDepth = 0;	
	//最后一次遇到的label是否NodeBegin
	bool bInNode = false;	

	CXmlNode* pCurNode = this;	//我也是节点，吼吼

	while( pCur < pEnd )
	{
		assert( iDepth >= 0 );
		assert( pCurNode != NULL );

		const char* pLastPos = pCur;

		if ( !FindLabel( pCur, pLabel, dwSizeLabel ) )
		{
			break;
		}
		switch ( *pLabel )
		{
		///////////////////////////////////////////////////////////////////////
		case '/':	//Node结束
			{
				if ( iDepth < 1 )
				{
					return false;
				}
			#ifdef _DEBUG
				// 找出 Unqualified Name
				const char *pUnqualified = std::find(pLabel + 1, pLabel + dwSizeLabel, ':');
				if (pUnqualified == pLabel + dwSizeLabel)
					pUnqualified = pLabel + 1; // 原本就是 Unqualified Name
				else
					++pUnqualified; // 指向 Unqualified Name 开始处

				//debug版检查node名称匹配
				if ( strncmp( pUnqualified, pCurNode->m_strName.c_str(), dwSizeLabel - (pUnqualified - pLabel) ) != 0
					|| pCurNode->m_strName.length() != dwSizeLabel - (pUnqualified - pLabel) )
				{
					return false;
				}
			#endif
				if ( bInNode )
				{
					assert( pCurNode != NULL );
					//提取Node内容
					pCurNode->m_strContent.assign( pLastPos, pLabel - pLastPos - 1 );
					bInNode = false;
				}
				pCurNode = pCurNode->GetParent();
				--iDepth;
			}
			break;
		///////////////////////////////////////////////////////////////////////
		case '?':	//xml定义块
			{
				const size_t cchEnd = strlen(XML_DEFINE_END);
			#ifdef _DEBUG
				//debug版做详细检查
				const size_t cchBegin = strlen(XML_DEFINE_BEGIN);
				if (dwSizeLabel < (cchBegin + cchEnd))
					return false; // Label 长度不足
				if ( strncmp( pLabel, XML_DEFINE_BEGIN, cchBegin) != 0 )
				{
					return false; // 起始字符不符
				}
				if (strncmp(pLabel + dwSizeLabel - cchEnd, XML_DEFINE_END, cchEnd) != 0)
					return false; // 结束字符不符
			#endif
				// 清空属性表，防止读入坏的 XML 文件时属性越积越多
				ClearAttributes();

				// 解析结束字符之前的 Label
				ParseLabel(this, pLabel, dwSizeLabel - cchEnd);
				assert(m_strName == XML_DEFINE_BEGIN);
			}
			break;
		///////////////////////////////////////////////////////////////////////
		case '!':	//注释块
			{
			#ifdef _DEBUG
				//debug版做详细检查
				if ( strncmp( pLabel, XML_COMMENT_BEGIN, strlen( XML_COMMENT_BEGIN ) ) != 0 )
				{
					return false;
				}
			#endif
				CXmlNode* pComment = pCurNode->AddChild();
				pComment->m_strName.assign( pLabel, dwSizeLabel );
				pComment->SetEmpty( true );
			#ifdef _DEBUG
				if ( pComment->m_strName.rfind( XML_COMMENT_END ) != dwSizeLabel - strlen( XML_COMMENT_END ) )
				{
					return false;
				}
			#endif
			}
			break;
		///////////////////////////////////////////////////////////////////////
		default:	//节点开始
			{
				//提取第一个空格之前的字符串内容作为节点名，节点名不允许为空
				CXmlNode* pNode = pCurNode->AddChild();

				ParseLabel( pNode, pLabel, dwSizeLabel );

				//如果label最右边为'/'则代表这是个空节点
				if ( *( pLabel + dwSizeLabel - 1 ) == '/' )
				{
					pNode->SetEmpty( true );
				}
				else
				{
					//Node开始
					pCurNode = pNode;
					bInNode = true;
					++iDepth;
				}
			}
		}
	} // while( pCur < pEnd )

	if ( iDepth != 0 )
	{
		return false;
	}
	assert( pCurNode == this );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//定位下一个label
//一对尖括弧<>括起来的内容称为一个label
//pBegin	[int,out]	开始查找的字符串，返回label之后的第一个字符指针
//pLabel	[out]		返回label内容指针
//dwLabelLen[out]		返回label长度
//return				是否成功
///////////////////////////////////////////////////////////////////////////////
bool CXmlFile::FindLabel( const char* &pBegin, const char* &pLabel, size_t &dwLabelLen )
{
	assert( pBegin != NULL );

	const char* pLeft = strchr( pBegin, '<' );
	if ( NULL == pLeft )
	{
		return false;
	}

	pLabel = pLeft + 1;
	const char* pRight = strchr( pLabel, '>' );
	if ( NULL == pRight )
	{
		return false;
	}
	dwLabelLen = pRight - pLabel;
	pBegin = pRight + 1;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//解析label
//label字符串中第一个空格之前的为名字，其余是attribute信息
//pNode			[in]	节点指针
//pLabel		[in]	label起始指针
//dwSizeLabel	[in]	label长度
///////////////////////////////////////////////////////////////////////////////
void CXmlFile::ParseLabel( CXmlNode* pNode, const char* pLabel, size_t dwSizeLabel )
{
	assert( pNode != NULL );
	assert( pLabel != NULL );
	
	const char *const pEnd = pLabel + dwSizeLabel;

	// 取出名字，要 Unqualified Name
	const char* pCur = pLabel;
	const char* pStart = pLabel;
	while ( pCur < pEnd && *pCur != ' ' && *pCur != '/' )
	{
		if (*pCur++ == ':')
			pStart = pCur;
	}
	pNode->m_strName.assign( pStart, pCur - pStart );

	if ( pCur < pEnd && *pCur == ' ' )
	{
		//取属性
		while ( pCur < pEnd )
		{
			while ( pCur < pEnd && *pCur == ' ' )
			{
				++pCur;
			}
			//属性名
			const char* pAttrName = pCur;
			while ( pCur < pEnd && *pCur != ' ' && *pCur != '=' && *pCur != '/' )
			{
				++pCur;
			}
			size_t dwSizeAttrName = pCur - pAttrName; 

			//双引号中间的是属性值
			pCur = (const char*)memchr( pCur, '"', pEnd - pCur );
			if ( NULL == pCur )
			{
				return;
			}
			const char* pAttrValue = ++pCur;
			pCur = (const char*)memchr( pCur, '"', pEnd - pCur );
			if ( NULL == pCur )
			{
				return;
			}
			size_t dwSizeAttrValue = pCur - pAttrValue;

			CXmlAttribute* pAttribute = new CXmlAttribute;
			pAttribute->m_strName.assign( pAttrName, dwSizeAttrName );
			pAttribute->m_strValue.assign( pAttrValue, dwSizeAttrValue );
			pNode->m_listAttribute.push_back( pAttribute );
			++pCur;
		}
	}
}
