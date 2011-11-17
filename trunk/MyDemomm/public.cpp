#include "public.h"
#include "GameApp.h"

#include <stdlib.h>


float Lerp(float a, float b, float t)
{
	return a - (a*t) + (b*t);
}

float GetRandomFloat(float lowBound, float highBound)
{
	if( lowBound >= highBound ) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f; 

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound; 
}

float Min(float a, float b)
{
	return a<b?a:b; 
}

float Min(float a, float b, float c)
{
	return a<b?Min(a,c):Min(b,c); 
}

float Max(float a, float b)
{
	return a>b?a:b; 
}

float Max(float a, float b, float c)
{
	return a>b?Max(a,c):Max(b,c); 
}



char * UnicodeToANSI( const wchar_t* str )
{
	char* result;
	int textlen;
	textlen = WideCharToMultiByte( CP_ACP, 0, str, -1, NULL, 0, NULL, NULL );
	result =(char *)malloc((textlen+1)*sizeof(char));
	memset( result, 0, sizeof(char) * ( textlen + 1 ) );
	WideCharToMultiByte( CP_ACP, 0, str, -1, result, textlen, NULL, NULL );
	return result;
}


wchar_t * UTF8ToUnicode( const char* str )
{
	int textlen ;
	wchar_t * result;
	textlen = MultiByteToWideChar( CP_UTF8, 0, str,-1, NULL,0 );  
	result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t));  
	memset(result,0,(textlen+1)*sizeof(wchar_t));  
	MultiByteToWideChar(CP_UTF8, 0,str,-1,(LPWSTR)result,textlen );  
	return result;  
}

char* UTF8ToANSI(const char* str)
{	
	wchar_t* temp = UTF8ToUnicode(str);
	char* res = UnicodeToANSI(temp);
	delete []temp;
	return res;
}


// int LoadToMyBuffer(const char* fileName,char** ppMyBuffer)
// {
// 	int fh = _open( fileName, _O_BINARY | _O_RDONLY);
// 	if (fh == -1)
// 	{
// 		assert(false);
// 		return -1;
// 	}
// 
// 	DWORD dwReadLen = _filelength(fh);
// 	if (dwReadLen == 0)
// 	{
// 		assert(false);
// 		return -1;
// 	}
// 
// 	char* pBuffer = new char[dwReadLen];
// 	if (!pBuffer)
// 	{
// 		_close(fh);
// 		return -1;
// 	}
// 
// 	if ( _read(fh,pBuffer,dwReadLen) <= 0 )
// 	{
// 		delete[] pBuffer;
// 		_close(fh);
// 		return -1;
// 	}
// 	_close(fh);
// 
// 	*ppMyBuffer = pBuffer;
// 	return dwReadLen;
// }
// 
// 
// DWORD GetDWORD(char** pMyBuffer)
// {
// 	DWORD dwTemp = 0;
// 	memcpy( (char*)&dwTemp, *pMyBuffer, sizeof(DWORD) );
// 	*pMyBuffer += sizeof(DWORD);
// 	return dwTemp;
// }
// 
// BOOL GetBOOL(char** pMyBuffer)
// {
// 	BOOL dwTemp;
// 	memcpy( (char*)&dwTemp, *pMyBuffer, sizeof(BOOL) );
// 	*pMyBuffer += sizeof(BOOL);
// 	return dwTemp;
// }
// 
// std::string GetStr(char** pMyBuffer)
// {
// 	DWORD dwLenth = GetDWORD(pMyBuffer);
// 	assert(dwLenth < MAX_PATH);
// 	char szTemp[MAX_PATH] = {0};
// 	memcpy( szTemp, *pMyBuffer, dwLenth );
// 	*pMyBuffer += dwLenth;
// 	return szTemp;
// }




CRay::CRay(int x,int y) //根据屏幕上点击点计算拾取射线
{
	//获取视口的尺寸：
	D3DVIEWPORT9 vp; 
	g_pD3DDevice->GetViewport(&vp); 

	//获取变换矩阵：
	D3DXMATRIX proj; 
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &proj); 

	//计算射线： 
	float px = (2.0f * x  / vp.Width  - 1.0f) / proj._11; 
	float py = (-2.0f * y / vp.Height + 1.0f) / proj._22; 
	m_origin    = D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
	m_direction = D3DXVECTOR3(px, py, 1.0f);

}

CRay::~CRay()
{

}

void CRay::TransformToMeshWord(const D3DXMATRIX &matWorld) //将射线变换到某物体的世界坐标空间下
{
	//计算逆矩阵 
	D3DXMATRIX view,inverseView,inverseWord; 
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &view); 
	D3DXMatrixInverse(&inverseWord,0,&matWorld); 
	D3DXMatrixInverse(&inverseView,0,&view); 
	//inverse=inverseWord * inverseView;   //顺序错误
	D3DXMATRIX inverse = inverseView * inverseWord;  

	//变换射线 
	D3DXVec3TransformCoord(&m_origin,&m_origin,&inverse); 
	D3DXVec3TransformNormal(&m_direction,&m_direction,&inverse); 
	D3DXVec3Normalize(&m_direction,&m_direction); 

}

