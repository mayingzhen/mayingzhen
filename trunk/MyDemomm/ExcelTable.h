
#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <list>


typedef std::vector<char*>		CStlStringArray;
typedef std::vector<CStlStringArray*>	CStlStringTable;
#pragma warning(disable:4251)//ģ�����dll �ӿ�

/////////////////////////////////////////////////
//
//	class	CExcelTable
//
/////////////////////////////////////////////////
/////////////////////////////////////////////////
#if !defined(EXCEL_INVALID)
#define	EXCEL_INVALID	-1
#endif//#if !defined(EXCEL_INVALID)


#if !defined(EXCEL_STR_MAX)
#define	EXCEL_STR_MAX	256
#endif//#if !defined(EXCEL_STR_MAX)

class CExcelTextTable
{
private:
	int			m_nMaxLn;	//��ǰ�������
	int			m_nMaxCol;	//��ǰ�������

	int			m_nAssumedMaxLn;
	int			m_nAssumedMaxCol;

	CStlStringTable	m_Table;
	CStlStringArray m_pBufferArray;

	std::string m_szFileName;
	const char*  GetCell(int nLine,int nCol)const;

public:
	//ע�⣺
	// nLnMax,nColMax ����ָ��Ԥ��Ĵ�С������ʵ�Ĳ���Ҳû��ϵ��
	// ֻ����� ����ʵ����Ļ����ճ��ڴ��˷�
	//     ��ӽ���ʵֵ�Ļ����Լӿ�����ٶ�
	CExcelTextTable(int nLnMax=1,int nColMax=1);
	virtual ~CExcelTextTable();

	int GetNumLines()const{
		return (int)m_Table.size();
	}
	int	GetMaxLn()const {
		return m_nMaxLn;
	}
	int	GetMaxCol()const {
		return m_nMaxCol;
	}

	// ���������������
	void Reset();

	// ��ȡ���Ʊ��Ϊ������ŵ��ı��ļ��������ۼƶ�ȡ
	// �� nStartLine ָ����ʼ��ȡ���кţ���� <= 0 ��ʾ�ӵ�0�ж���
	// �� nEndLine ָ����ȡ����һ�к�Ϊֹ��������һ�У������ < 0 ��ʾһֱ����ĩ��
	// ��� nStartLine > 0 && nEndLine >= 0  �� nEndLine < nStartLine �Ļ� ����FALSE
	// returns	:	�ļ������ڻ������ݳ��緵��FALSE
	BOOL ReadExcelTable( LPCTSTR szName , int nStartLine = -1, int nEndLine = -1);

	// �õ�ָ��λ�õ�����
	// nSize	:	�õ��ĺ�׺����󳤶ȣ����С������ַ������ڶ�������������ʵ���ȣ���������������
	// ��û����д���� nData = ȱʡֵ, output = "";
	// 
	void Get( int nLn, int nCol, char* output, int nSize,LPCSTR szDefault = "")const;
	void Get( int nLn, int nCol, int &nData , int nDefault = -1)const;
	void Get( int nLn, int nCol, float &fData , float fDefault = 0.0f)const;
	void Get_BOOL( int nLn, int nCol, BOOL& bData, BOOL bDefault = FALSE)const;

	void Get( int nLn, int nCol, DWORD& dwOutData, DWORD dwDefault =  0xffffffff)const;

	// ���Ӷ�ȡ64bit�����Ĺ���
	void Get( int nLn, int nCol, __int64& n64OutData , __int64 n64Default = 0)const;
	void Get( int nLn, int nCol, unsigned __int64& un64OutData , unsigned __int64 un64Default = 0)const;

	// ����ĳ�е����ݲ�����
	// returns	:	-1Ϊû���ҵ�������Ϊ���е���ţ�ֻ���ҵ���һ��
	int	FindLn( int nCol, int nData )const;
	int	FindLn( int nCol, const char* szData )const;

	// ����Ƿ���ĳ��������ͬ����
	// returns	:	-1Ϊû���ظ��У�����Ϊ�ڶ�����ͬ�е����
	int CheckDup( int nCol )const;

	// ������ͬĳ��������ͬ���У��ҵ����������ݶ�������pnFind��
	// returns	:	�����ҵ��ĸ���
	int GetDupLns( int nCol, int nData, int* pnFind, int nCount )const;
	int GetDupLns( int nCol, const char* szData, int* pnFind, int nCount )const;
};
/////////////////////////////////////////////////
