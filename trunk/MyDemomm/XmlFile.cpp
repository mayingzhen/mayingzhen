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
//����
///////////////////////////////////////////////////////////////////////////////
CXmlNode::CXmlNode()
	: m_pParent( NULL )
	, m_bEmpty( false )
{
}

///////////////////////////////////////////////////////////////////////////////
//����
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
//����ӽڵ�
///////////////////////////////////////////////////////////////////////////////
CXmlNode* CXmlNode::AddChild()
{
	CXmlNode* pChild = new CXmlNode;

	pChild->m_pParent = this;
	m_listChild.push_back( pChild );

	return pChild;
}

///////////////////////////////////////////////////////////////////////////////
//����ڵ㣨�����ӽڵ㣩
//������������ﲻ����д�ļ�ʧ�ܵ����
///////////////////////////////////////////////////////////////////////////////
void CXmlNode::SaveNode( FILE* fp, int iDepth )
{
	assert( fp != NULL );

	//��ʼ��ǩ
	//����������tab��
	int iTabCount = iDepth;
	while ( iTabCount-- > 0 )
	{
		fputc( '	', fp );
	}
	fputc( '<', fp );
	fputs( m_strName.c_str(), fp );

	//����
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

	// ���� Label ���������ַ�
	if (iDepth < 0)
	{
		// XML ����飬Label ��Ҫ�ԡ�?������
		assert(strcmp(m_strName.c_str(), XML_DEFINE_BEGIN) == 0);
		fputs(XML_DEFINE_END, fp);
	}
	else if ( m_bEmpty )
	{
		//�սڵ㣨����ע�ͣ�
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
		//Ҷ�ڵ㣬��������д��ͬһ��
		fputs( m_strContent.c_str(), fp );
		fputs( "</", fp );
		fputs( m_strName.c_str(), fp );
		fputc( '>', fp );
		fputs( "\r\n", fp );
		return;
	}
	fputs( "\r\n", fp );
	//�ӽڵ�
	SaveChildNode( fp, iDepth );

	// XML ����鲻��Ҫ������ǩ
	if (iDepth < 0)
		return;

	//������ǩ
	//����������tab��
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
//�����ֽڵ�
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
//����
///////////////////////////////////////////////////////////////////////////////
CXmlFile::CXmlFile()
	: m_LastError( XPE_Unknown )
{
	Reset();
}

///////////////////////////////////////////////////////////////////////////////
//���ļ��м���
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
//���ڴ��м���
///////////////////////////////////////////////////////////////////////////////
bool CXmlFile::LoadFromMemory( const char* pBuffer, size_t dwBufferLen )
{
	Reset();

	return Parse( pBuffer, dwBufferLen );
}

///////////////////////////////////////////////////////////////////////////////
//���浽�ļ�
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
// ���ð�����������ڵ����� XML �ṹ
//
///////////////////////////////////////////////////////////////////////////////
void CXmlFile::Reset()
{
	// ɾ�������ӽڵ������
	ClearChild();
	ClearAttributes();

	// ���춨���
	m_strName = XML_DEFINE_BEGIN;
	CXmlAttribute *pAttr = AppendAttribute("version");
	assert(pAttr != NULL);
	pAttr->SetValue("1.0");
	pAttr = AppendAttribute("encoding");
	assert(pAttr != NULL);
	pAttr->SetValue("GB2312");

	// CXmlFile ��һ������� CXmlNode����������������� XML ����飬ͬʱ����
	// �ǿսڵ�
}

///////////////////////////////////////////////////////////////////////////////
//����xml
//pInput	[in]	������xml�ַ���
//dwSize	[in]	������xml�ַ�������
//return			�Ƿ�ɹ�
///////////////////////////////////////////////////////////////////////////////
bool CXmlFile::Parse( const char* pInput, size_t dwSize )
{
	assert( pInput != NULL );

	const char* pCur = pInput;
	const char* pEnd = pInput + dwSize;

	const char* pLabel = NULL;
	size_t dwSizeLabel = 0;

	//��ǰ���������
	int iDepth = 0;	
	//���һ��������label�Ƿ�NodeBegin
	bool bInNode = false;	

	CXmlNode* pCurNode = this;	//��Ҳ�ǽڵ㣬���

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
		case '/':	//Node����
			{
				if ( iDepth < 1 )
				{
					return false;
				}
			#ifdef _DEBUG
				// �ҳ� Unqualified Name
				const char *pUnqualified = std::find(pLabel + 1, pLabel + dwSizeLabel, ':');
				if (pUnqualified == pLabel + dwSizeLabel)
					pUnqualified = pLabel + 1; // ԭ������ Unqualified Name
				else
					++pUnqualified; // ָ�� Unqualified Name ��ʼ��

				//debug����node����ƥ��
				if ( strncmp( pUnqualified, pCurNode->m_strName.c_str(), dwSizeLabel - (pUnqualified - pLabel) ) != 0
					|| pCurNode->m_strName.length() != dwSizeLabel - (pUnqualified - pLabel) )
				{
					return false;
				}
			#endif
				if ( bInNode )
				{
					assert( pCurNode != NULL );
					//��ȡNode����
					pCurNode->m_strContent.assign( pLastPos, pLabel - pLastPos - 1 );
					bInNode = false;
				}
				pCurNode = pCurNode->GetParent();
				--iDepth;
			}
			break;
		///////////////////////////////////////////////////////////////////////
		case '?':	//xml�����
			{
				const size_t cchEnd = strlen(XML_DEFINE_END);
			#ifdef _DEBUG
				//debug������ϸ���
				const size_t cchBegin = strlen(XML_DEFINE_BEGIN);
				if (dwSizeLabel < (cchBegin + cchEnd))
					return false; // Label ���Ȳ���
				if ( strncmp( pLabel, XML_DEFINE_BEGIN, cchBegin) != 0 )
				{
					return false; // ��ʼ�ַ�����
				}
				if (strncmp(pLabel + dwSizeLabel - cchEnd, XML_DEFINE_END, cchEnd) != 0)
					return false; // �����ַ�����
			#endif
				// ������Ա���ֹ���뻵�� XML �ļ�ʱ����Խ��Խ��
				ClearAttributes();

				// ���������ַ�֮ǰ�� Label
				ParseLabel(this, pLabel, dwSizeLabel - cchEnd);
				assert(m_strName == XML_DEFINE_BEGIN);
			}
			break;
		///////////////////////////////////////////////////////////////////////
		case '!':	//ע�Ϳ�
			{
			#ifdef _DEBUG
				//debug������ϸ���
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
		default:	//�ڵ㿪ʼ
			{
				//��ȡ��һ���ո�֮ǰ���ַ���������Ϊ�ڵ������ڵ���������Ϊ��
				CXmlNode* pNode = pCurNode->AddChild();

				ParseLabel( pNode, pLabel, dwSizeLabel );

				//���label���ұ�Ϊ'/'��������Ǹ��սڵ�
				if ( *( pLabel + dwSizeLabel - 1 ) == '/' )
				{
					pNode->SetEmpty( true );
				}
				else
				{
					//Node��ʼ
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
//��λ��һ��label
//һ�Լ�����<>�����������ݳ�Ϊһ��label
//pBegin	[int,out]	��ʼ���ҵ��ַ���������label֮��ĵ�һ���ַ�ָ��
//pLabel	[out]		����label����ָ��
//dwLabelLen[out]		����label����
//return				�Ƿ�ɹ�
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
//����label
//label�ַ����е�һ���ո�֮ǰ��Ϊ���֣�������attribute��Ϣ
//pNode			[in]	�ڵ�ָ��
//pLabel		[in]	label��ʼָ��
//dwSizeLabel	[in]	label����
///////////////////////////////////////////////////////////////////////////////
void CXmlFile::ParseLabel( CXmlNode* pNode, const char* pLabel, size_t dwSizeLabel )
{
	assert( pNode != NULL );
	assert( pLabel != NULL );
	
	const char *const pEnd = pLabel + dwSizeLabel;

	// ȡ�����֣�Ҫ Unqualified Name
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
		//ȡ����
		while ( pCur < pEnd )
		{
			while ( pCur < pEnd && *pCur == ' ' )
			{
				++pCur;
			}
			//������
			const char* pAttrName = pCur;
			while ( pCur < pEnd && *pCur != ' ' && *pCur != '=' && *pCur != '/' )
			{
				++pCur;
			}
			size_t dwSizeAttrName = pCur - pAttrName; 

			//˫�����м��������ֵ
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
