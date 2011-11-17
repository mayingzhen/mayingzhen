
#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <list>


typedef std::vector<char*>		CStlStringArray;
typedef std::vector<CStlStringArray*>	CStlStringTable;
#pragma warning(disable:4251)//模板类的dll 接口

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
	int			m_nMaxLn;	//当前最大行数
	int			m_nMaxCol;	//当前最大列数

	int			m_nAssumedMaxLn;
	int			m_nAssumedMaxCol;

	CStlStringTable	m_Table;
	CStlStringArray m_pBufferArray;

	std::string m_szFileName;
	const char*  GetCell(int nLine,int nCol)const;

public:
	//注意：
	// nLnMax,nColMax 用来指定预测的大小，和真实的不符也没关系，
	// 只是如果 〉真实情况的话会照成内存浪费
	//     最接近真实值的话可以加快加载速度
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

	// 清除表内所有内容
	void Reset();

	// 读取以制表符为间隔符号的文本文件，可以累计读取
	// 用 nStartLine 指定开始读取的行号，如果 <= 0 表示从第0行读起
	// 用 nEndLine 指定读取到哪一行号为止（包含这一行），如果 < 0 表示一直读到末行
	// 如果 nStartLine > 0 && nEndLine >= 0  但 nEndLine < nStartLine 的话 返回FALSE
	// returns	:	文件不存在或者数据超界返回FALSE
	BOOL ReadExcelTable( LPCTSTR szName , int nStartLine = -1, int nEndLine = -1);

	// 得到指定位置的数据
	// nSize	:	得到的后缀的最大长度，如果小于输出字符串（第二个参数）的真实长度，则会出现致命错误
	// 若没有填写，则 nData = 缺省值, output = "";
	// 
	void Get( int nLn, int nCol, char* output, int nSize,LPCSTR szDefault = "")const;
	void Get( int nLn, int nCol, int &nData , int nDefault = -1)const;
	void Get( int nLn, int nCol, float &fData , float fDefault = 0.0f)const;
	void Get_BOOL( int nLn, int nCol, BOOL& bData, BOOL bDefault = FALSE)const;

	void Get( int nLn, int nCol, DWORD& dwOutData, DWORD dwDefault =  0xffffffff)const;

	// 增加读取64bit变量的功能
	void Get( int nLn, int nCol, __int64& n64OutData , __int64 n64Default = 0)const;
	void Get( int nLn, int nCol, unsigned __int64& un64OutData , unsigned __int64 un64Default = 0)const;

	// 根据某列的内容查找行
	// returns	:	-1为没有找到，否则为该行的序号，只能找到第一个
	int	FindLn( int nCol, int nData )const;
	int	FindLn( int nCol, const char* szData )const;

	// 检查是否有某列内容相同的行
	// returns	:	-1为没有重复行，否则为第二个相同行的序号
	int CheckDup( int nCol )const;

	// 查找相同某列内容相同的行，找到的所有内容都保存在pnFind中
	// returns	:	返回找到的个数
	int GetDupLns( int nCol, int nData, int* pnFind, int nCount )const;
	int GetDupLns( int nCol, const char* szData, int* pnFind, int nCount )const;
};
/////////////////////////////////////////////////
