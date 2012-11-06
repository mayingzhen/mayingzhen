#ifndef C_PUBLIC__HH__
#define C_PUBLIC__HH__

//#include <d3d9.h>
#include <d3dx9.h>
//#include <dxerr9.h>

#include <assert.h>
#include <atlstr.h > // CString

#include "Debug.h"


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


/// Handles Win32 calls returing HRESULT
#define V(x) \
{ \
	HRESULT hr = x; \
	if (FAILED(hr)) \
		assert(false); \
}

float Lerp(float a, float b, float t);
float GetRandomFloat(float lowBound, float highBound);
char* UTF8ToANSI(const char* str);

float Min(float a, float b);
float Min(float a, float b, float c);
float Max(float a, float b);
float Max(float a, float b, float c);


// clamp variables of any type
template<typename Type>
inline Type Clamp(const Type &A, const Type &Min, const Type &Max)
{
	if (A < Min) return Min;
	if (A > Max) return Max;
	return A;
}

template<typename Type>
inline Type Clamp(const Type &Min, const Type &Max)
{
	Type A;
	if (A < Min) return Min;
	if (A > Max) return Max;
	return A;
}

template<class T>
void Lerp(T& out, const T& v0, const T& v1, float factor)
{
	out = v0 * (1.0f - factor) + v1 * factor;
}

template<class T>
T Lerp(const T& v0, const T& v1, float factor)
{
	return v0 * (1.0f - factor) + v1 * factor;
}

// time in seconds
inline double GetAccurateTime(void)
{
	__int64 iCurrentTime = 0;
	__int64 iFrequency = 1;
	QueryPerformanceFrequency((LARGE_INTEGER*)&iFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&iCurrentTime);
	return (double)iCurrentTime / (double)iFrequency;
}

inline float DeltaTimeUpdate(double &fLastUpdate)
{
	double fTimeNow = GetAccurateTime();
	//DWORD fime = GetTickCount();
	float fDeltaTime = 40.0f * Clamp((float)(fTimeNow - fLastUpdate), 0.0f, 1.0f);
	fLastUpdate = fTimeNow;
	return fDeltaTime;
}


// returns true if virtual key is down
inline bool GetKeyDown(int iVirtualKey)
{
	//if(GetFocus()!=GetAppBase()->GetHWND()) return false;
	return (GetKeyState(iVirtualKey) & 0xfe) ? true : false;
}

// returns true if mouse button VK_LBUTTON / VK_RBUTTON / VK_MBUTTON is down
inline bool GetMouseDown(int iVirtualKey)
{
	//if(GetFocus()!=GetAppBase()->GetHWND()) return false;
	return (GetAsyncKeyState(iVirtualKey) & 0x8000) ? true : false;
}

#define DegreeToRadian(fDegrees) ((3.14159265f/180.0f)*fDegrees)

#define PROFILE_LABEL_PUSH(X) { wchar_t buf[128]; size_t outCount=0; mbstowcs_s(&outCount, buf, X, _countof(buf)-1); D3DPERF_BeginEvent(0xff00ff00, buf); }
#define PROFILE_LABEL_POP(X) D3DPERF_EndEvent();


// int LoadToMyBuffer(const char* fileName,char** ppMyBuffer);
// WORD GetWORD(char** pMyBuffer);
// DWORD GetDWORD(char** pMyBuffer);
// BOOL GetBOOL(char** pMyBuffer);
// std::string GetStr(char** pMyBuffer);





// D3DMATERIAL9 GetD3DMATERIAL(char** pMyBuffer)
// {
// 	D3DMATERIAL9 temp;
// 	memcpy( (char*)&temp, *pMyBuffer, sizeof(D3DMATERIAL9) );
// 	*pMyBuffer += sizeof(D3DMATERIAL9);	
// 	return temp;
// }
// 
// D3DXVECTOR3 GetD3DXVECTOR3(char** pMyBuffer)
// {
// 	D3DXVECTOR3 temp;
// 	memcpy( (char*)&temp, *pMyBuffer, sizeof(D3DXVECTOR3) );
// 	*pMyBuffer += sizeof(D3DXVECTOR3);	
// 	return temp;
// }


// D3DXMATRIX GetD3DXMATRIX(char** pMyBuffer)
// {
// 	float fMat[16] = {0};
// 	memcpy( (char*)fMat, *pMyBuffer, sizeof(float) * 16 );
// 	*pMyBuffer += sizeof(float) * 16;
// 	return fMat;   	
// }






class CRay 
{
public:
	D3DXVECTOR3 m_origin;    //起点
	D3DXVECTOR3 m_direction; //方向

public:
	CRay(int x,int y);	//根据屏幕上点击点计算拾取射线
	~CRay();
	void TransformToMeshWord(const D3DXMATRIX &matWorld); //将射线转换到物体空间坐标下
};




//单件模式基类
template<class T>
class CSingleton
{
public:
	static T& GetSingleton();  //取得实例	

protected:
	CSingleton(){}
	virtual ~CSingleton() {}	

private:
	//防止拷贝复制		
	CSingleton(const CSingleton&);
	CSingleton& operator= (const CSingleton&);
};


//取得实例
template<class T>
T& CSingleton<T>::GetSingleton()
{
	static T instance;  //局部静态对象    
	return instance;
}


#endif
