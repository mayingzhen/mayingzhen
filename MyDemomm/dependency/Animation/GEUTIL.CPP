#include <Windows.h>
#include <algorithm>
#include "GEUtil.h"
#include <stdio.h>


//Math g_Math(4096);

string GEUTIL::STR_EMPTY = "";

// 点和矩形碰撞检测
bool GEUTIL::DotInRect(int dotX, int dotY, int rectX, int rectY, 
					int rectWidth, int rectHeight)
{
	//点和区域碰撞检测
	if (dotX > rectX && dotX < rectX + rectWidth 
		&& dotY > rectY && dotY < rectY + rectHeight )
		return true;
	
	else 
		return false;
}

bool GEUTIL::DotInRect(float dotX, float dotY, float rectX, float rectY, 
			   float rectWidth, float rectHeight)
{
	//点和区域碰撞检测

	if (dotX > rectX && dotX < rectX + rectWidth 
		&& dotY > rectY && dotY < rectY + rectHeight )
		return true;

	else 
		return false;

}


// 两个矩形碰撞检测
bool GEUTIL::RectCollision(int rectAX,int rectAY,int rectAWidth,int rectAHeight,
					int rectBX,int rectBY,int rectBWidth,int rectBHeight)
{
	//矩形碰撞检测
	// 排除不碰撞的情况,就是碰撞了
	if ( (rectAX-rectBX >= rectBWidth) || (rectBX-rectAX >= rectAWidth) 
		|| (rectAY-rectBY >= rectBHeight) || (rectBY-rectAY >= rectAHeight) )
		return false;
	else 
		return true;

}

float GEUTIL::StrToFloat(const string& str)
{
	return (float)atof(str.c_str());
}

int GEUTIL::StrToInt_(const string& str)
{
	return atoi(str.c_str());
}

bool GEUTIL::StrToBool(const string& str)
{
	int i = atoi(str.c_str());
	if (i == 0)
		return false;
	else 
		return true;
}

string GEUTIL::IntToStr(int i)
{
	static char c[10];
	string str;
	itoa(i, c, 10);
	str = c;
	return str;
}

string GEUTIL::FloatToStr(float f, int bitAfterDot)
{
	//alala
	if(f == 0 && bitAfterDot == 0)
		return "0";
	//alala
	int decimal, sign;
	string s = _fcvt(f, bitAfterDot, &decimal, &sign);
	string result = "";
	if (sign != 0)
		result = "-";

	if (decimal > 0)
	{
		result += s.substr(0, decimal);
		if (bitAfterDot != 0)
			result += ".";
		result += s.substr(decimal);
	}
	else
	{
		result += "0.";
		int idecimal = -decimal;
		while (idecimal--)
		{
			result += "0";
		}
		result += s;
	}

	return result;
}

string GEUTIL::GetNextStrNotCmt(ifstream& fin)
{
	string str;
	fin >> str;

	while(str.substr(0, 2) == "//")
	{
		getline(fin, str);
		fin >> str;
	}

	return str;
}

string GEUTIL::GetThisLineRest(ifstream& fin)
{
	string s1;
	fin >> s1;
	string temp;
	getline(fin, temp);
	s1 += temp;

	// 将里面的 \n 变成回车符
	string s2 = "";
	int pos1 = 0;
	for (;;)
	{
		pos1 = s1.find_first_of('\\');
		if (pos1 == -1)
		{	// 没有回车了
			s2 += s1;
			break;			
		}

		char c = s1[pos1+1];
		if (s1[pos1+1] == 'n')
		{
			// \后面确实跟着n
			s2 += s1.substr(0, pos1);
			s2 += '\n';
			s1 = s1.substr(pos1+2);		
		}
		else
		{
			// \后面不是n
			s2 += s1.substr(0, pos1+1);
			s1 = s1.substr(pos1+1);		
		}
	} 
	return s2;
}


string GEUTIL::GetNextStrStopBy(ifstream& fin, const string& stopStr)
{
	string str = "";
	char c;

	// 得到第一个非中止字符
	while (!fin.eof())
	{
		fin.read(&c, 1);
		if (stopStr.find_first_of(c) == string::npos)
		{
			str += c;
			break;
		}
	}

	// 直到遇到终止字符
	while (!fin.eof())
	{
		fin.read(&c, 1);
		if (stopStr.find_first_of(c) == string::npos)
		{
			str += c;
		}
		else
			break;
	}

	return str;
}


string GEUTIL::ToLowerCase(const string& str)
{
	char* s = new char[str.length()+1];
	memcpy(s, str.c_str(), str.length());
	s[str.length()] = '\0';

	int offset = 'a' - 'A';
	for (unsigned int i = 0; i < str.length(); ++i)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
			s[i] += offset;
	}

	string result = s;
	delete [] s;
	return result;
}

string GEUTIL::ToUpperCase(const string& str)
{
	char* s = new char[str.length()+1];
	memcpy(s, str.c_str(), str.length());
	s[str.length()] = '\0';

	int offset = 'a' - 'A';
	for (unsigned int i = 0; i < str.length(); ++i)
	{
		if (s[i] >= 'a' && s[i] <= 'z')
			s[i] -= offset;
	}

	string result = s;
	delete [] s;
	return result;
}


void GEUTIL::WriteStrWithLineSymbol(ofstream& fout, string str)
{
	string s1 = "";
	for (;;)
	{
		int pos = str.find_first_of('\n');
		if (pos != -1)
		{
			s1 += str.substr(0, pos);
			s1 += "\\n";
			str = str.substr(pos+1);
		}
		else
		{
			s1 += str;
			break;
		}
	}

	fout << s1;
}


void GEUTIL::ReadWholeFile(string filename, string& content)
{
	// 这里非常恼火,我们规定文件的最后结尾一定要是一个回车
	content="";
	ifstream f(filename.c_str());
	f.seekg(0, ios_base::end);
	int len = f.tellg();
	f.seekg(0, ios_base::beg);
	char * buf = (char *)malloc( sizeof(char) * (len + 1) );
	f.read(buf, len);
	buf[len] = '\0';
	content = buf;
	int index = content.find_last_of('\n');
	content = content.substr(0, index);
	delete buf;

	//FILE *fp;
	//long len;
	//char *buf;
	//fp=fopen(filename.c_str(),"r");
	//fseek(fp,0,SEEK_END); //go to end
	//len=ftell(fp); //get position at end (length)
	//fseek(fp,0,SEEK_SET); //go to beg.
	//buf=(char *)malloc(len); //malloc buffer
	//fread(buf,len,1,fp); //read into buffer
	//fclose(fp);
	//content = buf;
	//delete buf;
}



vector<string>& GEUTIL::FindFilesInDir(string sRootDir, vector<string>& vFileName, const string& sPostfix, const string& sPrefix)
{
	WIN32_FIND_DATA fileData;
	HANDLE hFind;

	// 补全根目录名
	if (sRootDir[sRootDir.size()-1] != '\\')
		sRootDir += "\\*";
	else
		sRootDir += "*";

	// 开始第一个文件查找
	hFind = FindFirstFile(sRootDir.c_str(), &fileData);

	//如果是目录
	if( (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		&& strcmp(fileData.cFileName, ".") && strcmp(fileData.cFileName, "..") )		
	{
		// 下一层搜索，递归实现
		int i = sRootDir.find_last_of('\\');
		string tmpPath = sRootDir.substr(0, i+1) + fileData.cFileName;
		string tmpPrefix = sPrefix + fileData.cFileName + '\\';
		FindFilesInDir(tmpPath, vFileName, sPostfix, tmpPrefix );
	}
	else if( strcmp(fileData.cFileName, ".") && strcmp(fileData.cFileName, "..") )
	{
		// 非目录，且后缀名相同
		string fileName = sPrefix + fileData.cFileName;
		if (sPostfix.length() == 0 || (fileName.length() > sPostfix.length() && fileName.substr(fileName.length()-sPostfix.length()) == sPostfix))
			vFileName.push_back(sPrefix + fileData.cFileName);
	}

	// 以下逻辑和第一次搜索一样
	while (true)
	{
		if (FindNextFile(hFind, &fileData))
		{
			if( (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& strcmp(fileData.cFileName, ".") && strcmp(fileData.cFileName, "..") )		
			{
				int i = sRootDir.find_last_of('\\');
				string tmpPath = sRootDir.substr(0, i+1) + fileData.cFileName;
				string tmpPrefix = sPrefix + fileData.cFileName + '\\';
				FindFilesInDir(tmpPath, vFileName, sPostfix, tmpPrefix );
			}
			else if( strcmp(fileData.cFileName, ".") && strcmp(fileData.cFileName, "..") )
			{
				string fileName = sPrefix + fileData.cFileName;
				if (sPostfix.length() == 0 || (fileName.length() > sPostfix.length() && fileName.substr(fileName.length()-sPostfix.length()) == sPostfix))
					vFileName.push_back(sPrefix + fileData.cFileName);
			}
		}
		else
		{
			if( GetLastError() == ERROR_NO_MORE_FILES )			//Normal Finished
			{
				break;
			}
			else
				break;
		}
	}

	sort(vFileName.begin(), vFileName.end());

	return vFileName;
}


bool GEUTIL::Is2Power(int n)
{
	if (n <= 0)
		return false;

	unsigned int bits = 1;			// n 用2进制表示有几位
	while(true)
	{
		if ( (n >> bits) > 0 )
			bits++;
		else
			break;
	}

	if ( ( (n >> (bits-1)) << (bits-1) ) == n )
		return true;
	else 
		return false;
}
