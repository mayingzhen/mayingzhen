#include <stdio.h>
#include <assert.h>
#include <io.h>
//#include <afx.h>

#include "ExcelTable.h"




//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

/////////////////////////////////////////////////
const int ENCRYPT_CODE = 0x79;
/////////////////////////////////////////////////
const int MAX_COL_EXCEL = 0x1000;//excel ����������




/////////////////////////////////////////////////
//
//	class	CExcelTable
//
/////////////////////////////////////////////////
/////////////////////////////////////////////////


CExcelTextTable::CExcelTextTable(int nLnMax,int nColMax)
{
	m_nAssumedMaxLn		= nLnMax;
	if ( m_nAssumedMaxLn < 1 )
		m_nAssumedMaxLn = 1;
	m_nAssumedMaxCol	= nColMax;
	if ( m_nAssumedMaxCol < 1 )
		m_nAssumedMaxCol = 1;

	m_nMaxLn  = 0;
	m_nMaxCol = 0;
}


CExcelTextTable::~CExcelTextTable()
{
	Reset();
}

// ���������������

void CExcelTextTable::Reset()
{
	{
		CStlStringTable::iterator it = m_Table.begin();
		while( it != m_Table.end() )
		{
			delete (*it++);
		}
		m_Table.clear();
	}
	{
		CStlStringArray::iterator it = m_pBufferArray.begin();
		while( it != m_pBufferArray.end() )
		{
			delete [] (*it);
			++it;
		}
	}

	m_pBufferArray.clear();
  
	m_nMaxLn  = 0;
	m_nMaxCol = 0;
}
const char* EMPTY_EXCEL_TEXT = "";
const char* CExcelTextTable::GetCell(int nLine,int nCol)const
{
	if ( nLine >= 0 && nLine < (int)m_Table.size() )
	{
		const CStlStringArray& TableLine = *m_Table[nLine];
		if ( nCol >= 0 && nCol < (int)TableLine.size() )
			return TableLine[nCol];
	}
	return EMPTY_EXCEL_TEXT;
}


// ��ȡ���Ʊ��Ϊ������ŵ��ı��ļ��������ۼƶ�ȡ
// �� nStartLine ָ����ʼ��ȡ���кţ���� <= 0 ��ʾ�ӵ�0�ж���
// �� nEndLine ָ����ȡ����һ�к�Ϊֹ��������һ�У������ < 0 ��ʾһֱ����ĩ��
// ��� nStartLine > 0 && nEndLine >= 0  �� nEndLine < nStartLine �Ļ� ����FALSE
// returns	:	�ļ������ڻ������ݳ��緵��FALSE
BOOL CExcelTextTable::ReadExcelTable( LPCTSTR szName , 
									 int nStartLine ,//= -1,
									 int nEndLine //= -1
									 )
{
	if ( nStartLine > 0 && nEndLine >= 0 && nEndLine < nStartLine )
		return FALSE;// �Ƿ��Ĳ���

	int nFileLine = 0;

	int nCounter = 0;
	int ch;
	BOOL ret = TRUE;

	// �޸�Ϊ��CPU�����̵ķ���,��ǰCPU�ٶ���,�����Ѿ���������,���Լ��ٴ��̵�IO����,��������ȡ�ٶ�.
	FILE*  fp;
	int    fh;
	if (!(fp = fopen(szName,"rb")))
	{
#ifdef _DEBUG
		char szTemp[512];
		sprintf(szTemp, "CExcelTextTable::ReadExcelTable() Error (0) : fopen %s Error!\n", szName);
		OutputDebugString( szTemp );
#endif
		return false;
	}
	fh = fileno(fp);
	long length = filelength(fh);
	if ( 0 == length ) 
	{
#ifdef _DEBUG
		OutputDebugString( "CExcelTextTable::ReadExcelTable() Error (0) : filelength =0!" );
#endif
		fclose(fp);
		return false ;
	}

	assert( length < 100*1024*1024 );	// �����ļ�С��100MB
 
	char* pBuffer = new char[length+1];
	if( !pBuffer )
	{
#ifdef _DEBUG
		OutputDebugString( "CExcelTextTable::ReadExcelTable() Error (0) : Not enough memory!" );
#endif
		return FALSE;
	}
	memset( pBuffer, 0, length+1 );

	fseek( fp, 0, SEEK_SET );
	size_t nCount = fread( pBuffer, length, 1, fp );
	if( nCount!= 1 )
	{
		delete pBuffer; pBuffer = NULL;
#ifdef _DEBUG
		OutputDebugString( "CExcelTextTable::ReadExcelTable() Error (1) : Read failed!" );
#endif
		return FALSE;
	}
	fclose( fp );

	m_szFileName = szName;

	long lPos = 0;
	BOOL bEncrypt = FALSE;
	ch = pBuffer[lPos++];
	if( (ch&0x000000FF) == 0x80 )
	{	// ������
		bEncrypt = TRUE;
	}
	else
	{
		lPos = 0;
	}
	std::list< CStlStringArray* > lList;
	std::list< CStlStringArray* >::iterator it;

	int nCol = 0;
	char *szUnit[MAX_COL_EXCEL];
	//CStlStringArray* ptrLine = new CStlStringArray();
	char *szStart = pBuffer + lPos;
	while( lPos < length )
	{
		//if( bEncrypt )
		//	CharEncrypt( pBuffer[lPos] );

		ch = pBuffer[lPos];

		if( ch == 0x0A )
		{
			lPos++;
		}
		else if( ch == 0x0D ) // "\r\n"
		{
			pBuffer[lPos] = 0;
			pBuffer[lPos+1] = 0;

			if ( nStartLine > 0 )
			{
				nStartLine--;
			}
			else
			{
				assert( nCol<MAX_COL_EXCEL );
				szUnit[nCol++]=szStart;

				//ptrLine->push_back( szStart );
			}

			nCounter = 0;

			if( ret == FALSE )
				goto error;

			nFileLine++;
			if (nEndLine >= 0 && nFileLine > nEndLine )
				goto WELL_DONE_BYEBYE;

			CStlStringArray*	ptrLine = new CStlStringArray( szUnit,szUnit+nCol );
			nCol = 0;
			//szUnit[nCol++]=szStart;

			lList.push_back( ptrLine );
			//ptrLine = new CStlStringArray();
			lPos++;
			lPos++;
			szStart = pBuffer + lPos;
		}
		else if( ch == 0x09 ) // "\t"
		{
			pBuffer[lPos] = 0;

			if ( nStartLine <= 0 )
			{
				assert( nCol<MAX_COL_EXCEL );
				szUnit[nCol++]=szStart;
				//ptrLine->push_back( szStart );
			}

			nCounter = 0;

			if( ret == FALSE )
				goto error;

			lPos++;
			szStart = pBuffer + lPos;
		}
		else
		{
			lPos++;
		}
	}

	if( szStart - pBuffer +2 != length )
	{
		if ( nStartLine <= 0 )
		{
			//ret = SetCell( nLn, nCol, szStart );
			assert( nCol<MAX_COL_EXCEL );
			szUnit[nCol++]=szStart;
			//ptrLine->push_back( szStart );

			CStlStringArray*	ptrLine = new CStlStringArray( szUnit,szUnit+nCol );
			nCol = 0;

			lList.push_back( ptrLine );
			//ptrLine = new CStlStringArray();

		}
		if( ret == FALSE )
			goto error;
	}


	it = lList.begin();
	while( it != lList.end() )
	{
		CStlStringArray* pLine = (*it);
		//�����������
		if( m_nMaxCol < (int)pLine->size() )
			m_nMaxCol = (int)pLine->size();

		m_Table.push_back( pLine );
		++it;
	}
	//�Ѿ� ��m_nMaxLn����
	//�����������
	m_nMaxLn += lList.size();


WELL_DONE_BYEBYE:
	m_pBufferArray.push_back( pBuffer );
	//delete ptrLine;
	return TRUE;

error:
	it = lList.begin();
	while( it != lList.end() )
	{
		delete (*it);
		++it;
	}

	//delete ptrLine;
	delete pBuffer;
	pBuffer = NULL;
	return FALSE;
}


// �õ�ָ��λ�õ�����
// nSize	:	�õ��ĺ�׺����󳤶ȣ����С������ַ������ڶ�������������ʵ���ȣ���������������
// ��û����д���� nData = default, output = "";
void CExcelTextTable::Get( int nLn, int nCol, char* output, int nSize ,LPCSTR szDefault)const
{
	LPCSTR szCell = GetCell(nLn,nCol);
	if( szCell[0] != '\0' )
	{
		assert( strlen(szCell)<(unsigned int)nSize );
		//::OU_SafeStrCpyA(output,nSize, szCell);
		memcpy(output,szCell,nSize);
	}
	else
	{
		if ( szDefault )
		{
			assert( strlen(szDefault)<(unsigned int)nSize );
			//::OU_SafeStrCpyA(output,nSize, szDefault);
			memcpy(output,szDefault,nSize);
		}
		else
		{
			output[0] = 0;
		}
	}
}

void CExcelTextTable::Get_BOOL( int nLn, int nCol, BOOL& bData, BOOL bDefault) const
{
	// ֻ��д��true��1��ʱ�����true
	LPCSTR szCell = GetCell(nLn,nCol);
	if( szCell[0] == '\0' ) {
		bData = bDefault;
	} else if (!stricmp(szCell, "true")) {
		bData = TRUE;
		return;
	} else {
		if (atoi(szCell) == 1) {
			bData = TRUE;
			return;
		}
	}

	bData = FALSE;
}

void CExcelTextTable::Get( int nLn, int nCol, int &nData , int nDefault //= -1
						  )const
{
	LPCSTR szCell = GetCell(nLn,nCol);
	if( szCell[0] != '\0' )
	{
		nData = atoi( szCell );
	}
	else
	{
		nData = nDefault;
	}
}

void CExcelTextTable::Get( int nLn, int nCol, float &fData , float fDefault //= 0.0f
						  )const
{
	LPCSTR szCell = GetCell(nLn,nCol);
	if( szCell[0] != '\0' )
	{
		fData = (float)atof( szCell );
	}
	else
	{
		fData = fDefault;
	}
}

// added by Mao Haibin 2003-11-17
void CExcelTextTable::Get( int nLn, int nCol, DWORD& dwOutData, DWORD dwDefault //=  0xffffffff 
						  )const
{
	LPCSTR szCell = GetCell(nLn,nCol);
	if( szCell[0] != '\0' )
	{
		dwOutData = strtoul( szCell ,NULL,10);
	}
	else
	{
		dwOutData = dwDefault;
	}
}

//added by Mao Haibin 2004-6-15,	���Ӷ�ȡ64bit�����Ĺ���
void CExcelTextTable::Get( int nLn, int nCol, __int64& n64OutData , __int64 n64Default //= 0
							 )const
{
	LPCSTR szCell = GetCell(nLn,nCol);
	if( szCell[0] != '\0' )
	{
		n64OutData =  _atoi64( szCell );
	}
	else
	{
		n64OutData = n64Default;
	}
}

void CExcelTextTable::Get( int nLn, int nCol, unsigned __int64& un64OutData  , unsigned __int64 un64Default //= 0
							)const
{
	LPCSTR szCell = GetCell(nLn,nCol);
	if( szCell[0] != '\0' )
	{
		un64OutData = (unsigned __int64)_atoi64( szCell );
	}
	else
	{
		un64OutData = un64Default;
	}
}


// ����ĳ�е����ݲ�����
// returns	:	-1Ϊû���ҵ�������Ϊ���е����
int	CExcelTextTable::FindLn( int nCol, int nData )const
{
	int i=0;
	for ( CStlStringTable::const_iterator it = m_Table.begin();
		it != m_Table.end(); it++,i++ )
	{
		const CStlStringArray& TableLine = (**it);
		if ( (int)TableLine.size() > nCol
			&& atoi(TableLine[nCol]) == nData)
			return i;
	}

	return -1;
}
int	CExcelTextTable::FindLn( int nCol, const char* szData )const
{
	int i=0;
	for ( CStlStringTable::const_iterator it = m_Table.begin();
		it != m_Table.end(); it++,i++ )
	{
		const CStlStringArray& TableLine = (**it);
		if ( (int)TableLine.size() > nCol && 
			stricmp(TableLine[nCol] , szData)==0 )
		{

			return i;
		}
	}

	return -1;
}

// ����Ƿ���ĳ��������ͬ����
// returns	:	-1Ϊû���ظ��У�����Ϊ�ڶ�����ͬ�е����
int CExcelTextTable::CheckDup( int nCol )const
{
	int i=0;
	for ( CStlStringTable::const_iterator itA = m_Table.begin();
		itA != m_Table.end(); itA++,i++ )
	{
		int j=i+1;
		for ( CStlStringTable::const_iterator itB = itA+1;
			itB != m_Table.end(); itB++,j++ )
		{
			if( (**itA) == (**itB) )
				return j;
		}
	}
	return -1;
}

// ������ͬĳ��������ͬ���У��ҵ����������ݶ�������pnFind��
// returns	:	�����ҵ��ĸ���
int CExcelTextTable::GetDupLns( int nCol, int nData, int* pnFind, int nCount )const
{
	memset( pnFind, 0, sizeof(int)*nCount );
	int nCounter = 0;
	int i=0;
	for ( CStlStringTable::const_iterator it = m_Table.begin();
		it != m_Table.end(); it++,i++)
	{
		if ( (int)(**it).size() > nCol
			&& atoi((**it)[nCol]) == nData )
		{
			pnFind[nCounter] = i;
			nCounter++;
			if( nCounter == nCount )
				break;
		}
	}
	return nCounter;
}
int CExcelTextTable::GetDupLns( int nCol, const char* szData, int* pnFind, int nCount )const
{
	memset( pnFind, 0, sizeof(int)*nCount );
	int nCounter = 0;
	int i=0;
	for ( CStlStringTable::const_iterator it = m_Table.begin();
		it != m_Table.end(); it++,i++)
	{
		if ( (int)(**it).size() > nCol
			&& (**it)[nCol] == szData )
		{
			pnFind[nCounter] = i;
			nCounter++;
			if( nCounter == nCount )
				break;
		}
	}
	return nCounter;
}
/////////////////////////////////////////////////

