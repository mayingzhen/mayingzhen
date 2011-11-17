
#pragma once

#pragma warning(disable:4786)

#define GEMSG_ERRBOX(str)	MessageBox ( NULL, str, "GE Engine", MB_ICONERROR);

#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
/// Delete contents of vector. Uses resize(0) so memory is not de-allocated/re-allocated
#define SAFE_DELETE_VECTOR(p)	{ for(int i=0;i<p.size();i++) { if(p[i]) delete p[i]; } p.resize(0); }
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_RELEASE_VECTOR(p)	{ for(int i=0;i<p.size();++i) SAFE_RELEASE(p[i]); p.clear(); }




/// includes
//#include <WINDOWS.H>
//#include <WINDOWSX.H>


#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstddef>
using namespace std;

/** 工具类,提供一些有用的小函数 */
class GEUTIL
{
public:
	// 点和矩形碰撞检测
	static bool DotInRect(int dotX, int dotY, int rectX, int rectY, 
					int rectWidth, int rectHeight);

	static bool DotInRect(float dotX, float dotY, float rectX, float rectY, 
		float rectWidth, float rectHeight);


	// 两个矩形碰撞检测
	static bool RectCollision(int rectAX,int rectAY,int rectAWidth,int rectAHeight,
					int rectBX,int rectBY,int rectBWidth,int rectBHeight);

	static float StrToFloat(const string& str);

	static int StrToInt_(const string& str);

	static bool StrToBool(const string& str);


	static string IntToStr(int i);

	static string FloatToStr(float f, int bitAfterDot);


	static string GetNextStrNotCmt(ifstream& fin);

	static string GetThisLineRest(ifstream& fin);

	/** 得到流中合理字符串
	 *	@param [const string&] stopStr : 字符串不能包含stopStr中的字符，由这些字符表示字符串结束	
	 *	@note: 不支持注释
	 */
	static string GetNextStrStopBy(ifstream& fin, const string& stopStr);

	/* 将字符串单行输出，特点是将字符串中的"\n"变成"\\n"输出 */
	static void WriteStrWithLineSymbol(ofstream& fout, string str);
	
	static void ReadWholeFile(string filename, string& content);

	static string ToLowerCase(const string& str);
	static string ToUpperCase(const string& str);

	/** 搜索一个目录下所有文件（包括子目录）
	* @param [string] sRootDir: 搜索根目录
	* @param [vector<string>&] vFileName: 返回的文件列表
	* @param [const string&] sPostfix: 文件的后缀名，可以为空(表示不关心文件类型)
	* @param [const string&] sPrefix: 目录的前缀，调用函数的时候不用赋值
	* @return [vector<string>&] : 符合条件的在该目录下的所有文件名
	*/
	static vector<string>& FindFilesInDir(string sRootDir, vector<string>& vFileName, const string& sPostfix = "", const string& sPrefix = "");

	/** 是否是2的指数冥 */
	static bool Is2Power(int n);

public:
	static string STR_EMPTY;
};

