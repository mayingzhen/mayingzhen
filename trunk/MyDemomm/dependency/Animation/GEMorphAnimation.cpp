
#include "GEMorphAnimation.h"
#include "GEUTIL.H"
#include <fstream>
using namespace std;

GEMorphAnimationKey::GEMorphAnimationKey(const string& name, unsigned int nVertexs)
					: m_sName(name), m_nVertexs(nVertexs), m_vPositions(NULL), m_vNormals(NULL)
{

}


GEMorphAnimationKey::~GEMorphAnimationKey()
{
	delete [] m_vPositions;
	delete [] m_vNormals;
}


bool GEMorphAnimationKey::LoadFromSrc(float* pPositionSrc, float* pNormalSrc)
{
	float* _pPositionSrc = pPositionSrc;
	float* _pNormalSrc = pNormalSrc;

	if (_pPositionSrc == NULL || _pNormalSrc == NULL)
		return false;

	delete [] m_vPositions;
	delete [] m_vNormals;
	m_vPositions= new D3DXVECTOR3[m_nVertexs];
	m_vNormals	= new D3DXVECTOR3[m_nVertexs];	

	for (unsigned int i = 0; i < m_nVertexs; ++i)
	{
		m_vPositions[i].x = *(_pPositionSrc++);
		m_vPositions[i].y = *(_pPositionSrc++);
		m_vPositions[i].z = *(_pPositionSrc++);

		m_vNormals[i].x = *(_pNormalSrc++);
		m_vNormals[i].y = *(_pNormalSrc++);
		m_vNormals[i].z = *(_pNormalSrc++);
	}

	return true;
}

void GEMorphAnimationKey::PrepareMemory()
{
	m_vPositions= new D3DXVECTOR3[m_nVertexs];
	m_vNormals	= new D3DXVECTOR3[m_nVertexs];	
}

GEMorphAnimation::GEMorphAnimation()
{
	m_bPlaying = false;
	m_bLoop = false;
	m_fTimeScale = 1.f;
	//m_SourceKey = NULL;
	//m_TargetKey = NULL;

	m_CurrentPose.first = 0;
	m_CurrentPose.second = NULL;
}

GEMorphAnimation::~GEMorphAnimation()
{
	if (m_CurrentPose.second != NULL)
		delete m_CurrentPose.second;
}

bool GEMorphAnimation::AddKey(float time, GEMorphAnimationKey* key)
{
	if (time < 0 || key == NULL)
		return false;

	vector< GEMorphAnimationPair >::iterator itr = m_Keys.begin();
	while (itr != m_Keys.end())
	{
		if ((*itr).first > time)
			break;
		++itr;
	}
	m_Keys.insert(itr, make_pair(time, key));

	return true;
}

bool GEMorphAnimation::SetTimeScale(float timeScale)
{
	if (timeScale < 0)
		return false;

	m_fTimeScale = timeScale;
	return true;
}


float GEMorphAnimation::GetCurrentPoseKeyPartition() const
{
	return (m_CurrentPose.first - m_Keys.front().first)/(m_Keys.back().first - m_Keys.front().first);
}


bool GEMorphAnimation::Update(float deltaTime)
{
	// return if not playing
	if (!m_bPlaying)
		return false;

	if (m_CurrentPose.second == NULL)
	{
		m_CurrentPose.second = new GEMorphAnimationKey("CURRENT_POSE", m_Keys.front().second->GetVertexCount());
		m_CurrentPose.second->PrepareMemory();
	}

	// update animation time
	m_CurrentPose.first += deltaTime*m_fTimeScale;
	if (m_CurrentPose.first > m_Keys.back().first)	// if bigger then total length 
	{
		if (m_bLoop)
		{
			while(m_CurrentPose.first > m_Keys.back().first)
				m_CurrentPose.first -= m_Keys.back().first;
		}
		else
		{
			// end of animation
			// set to the end of animation
			for (unsigned int i = 0; i < m_CurrentPose.second->GetVertexCount(); ++i)
			{
				// positions
				const D3DXVECTOR3& pos = m_Keys.back().second->GetPosition(i);
				m_CurrentPose.second->SetPosition(i, pos.x, pos.y, pos.z);
				// Normals
				const D3DXVECTOR3& nor = m_Keys.back().second->GetNormal(i);
				m_CurrentPose.second->SetNormal(i, nor.x, nor.y, nor.z);
			}
			m_bPlaying = false;
			return true;
		}
	}

	for (unsigned int i = 0; i < m_Keys.size(); ++i)
	{
		if (m_Keys[i+1].first > m_CurrentPose.first)
		{
			m_SourceKey = m_Keys.begin()+i;
			m_TargetKey = m_Keys.begin()+i+1;
			break;
		}
	}
	
	// get the right animation
	float part = 0;
	if (m_TargetKey->first > m_SourceKey->first)
		part = (m_CurrentPose.first-m_SourceKey->first)/(m_TargetKey->first-m_SourceKey->first);
	else
		part = (m_CurrentPose.first+m_Keys.back().first-m_SourceKey->first)/(m_TargetKey->first+m_Keys.back().first-m_SourceKey->first);

	for (unsigned int i = 0; i < m_CurrentPose.second->GetVertexCount(); ++i)
	{
		// fill all vertexs
		// rule : current = (1-part)*source + part*target
		// positions
		const D3DXVECTOR3& srcPos = m_SourceKey->second->GetPosition(i);
		const D3DXVECTOR3& tgtPos = m_TargetKey->second->GetPosition(i);
		D3DXVECTOR3 curPos = srcPos*(1-part)+tgtPos*part;
		m_CurrentPose.second->SetPosition(i, curPos.x, curPos.y, curPos.z);
		// Normals
		const D3DXVECTOR3& srcNor = m_SourceKey->second->GetNormal(i);
		const D3DXVECTOR3& tgtNor = m_TargetKey->second->GetNormal(i);
		D3DXVECTOR3 curNor;// = (srcNor*(1-part)+tgtNor*part).GetNormalized();
		D3DXVec3Normalize(&curNor, &(srcNor*(1-part)+tgtNor*part) );
		m_CurrentPose.second->SetNormal(i, curNor.x, curNor.y, curNor.z);
	}

	return true;
}	
	

void GEMorphAnimation::Play(bool loop, float startTime)
{
	m_bPlaying	= true;
	m_bLoop = loop;
	m_CurrentPose.first	= startTime;
	
	if ( m_Keys.size() > 0 
		&& (m_CurrentPose.first < 0 || m_CurrentPose.first > m_Keys.back().first) )
	{	// 时间不对
		m_bPlaying = false;
	}
}


void GEMorphAnimation::Pause()
{
	m_bPlaying = false;
}


void GEMorphAnimation::Resume()
{
	m_bPlaying = true;
}


/** 动画是否处于末尾帧 */
bool GEMorphAnimation::IsEnd() const
{
 	if (!m_bLoop && m_CurrentPose.first >= m_Keys.back().first)
 		return true;
 	else
 		return false;

// 	if (!m_bLoop && m_CurrentPose.first/m_fTimeScale >= GetAnimationLength())
// 		return true;
// 	else 
// 		return false;

}

/** 得到经过缩放的长度 */
float GEMorphAnimation::GetAnimationLength() const
{
	return m_Keys.back().first/m_fTimeScale;
}

/** 得到当前播放的位置 */
float GEMorphAnimation::GetCurrentTime() const
{
	return m_CurrentPose.first/m_fTimeScale;
};



GEMorphAnimationController::GEMorphAnimationController()
							: m_pActiveAnimation(NULL)
{
	
}

GEMorphAnimationController::~GEMorphAnimationController()
{
	map<string, GEMorphAnimationKey*>::iterator itr = m_AnimationKeySet.begin();
	while (itr != m_AnimationKeySet.end())
	{
		delete itr->second;
		++itr;
	}
	m_AnimationKeySet.clear();

	// 别的都不是指针，会自行清理
}

unsigned int GEMorphAnimationController::AddAnimation(GEMorphAnimation ani)
{
	m_AnimationSet.insert(make_pair(ani.GetName(), ani));
	return static_cast<unsigned int>(m_AnimationSet.size());
}

bool GEMorphAnimationController::Update(float deltaTime)
{
	if (m_pActiveAnimation != NULL)
		return m_pActiveAnimation->Update(deltaTime);
	else
		return false;
}

bool GEMorphAnimationController::PlayAnimation(const string& name, bool loop, float startTime)
{
	map<string, GEMorphAnimation>::iterator itr = m_AnimationSet.find(name);
	if (itr != m_AnimationSet.end())
	{
		m_pActiveAnimation = &itr->second;
		itr->second.Play(loop, startTime);
		return true;
	}
	else
	{
		m_pActiveAnimation = NULL;
		return false;
	}
}

bool GEMorphAnimationController::StopActiveAnimation()
{
	if (m_pActiveAnimation != NULL)
	{
		m_pActiveAnimation->Pause();
		m_pActiveAnimation = NULL;
		return true;
	}
	else
        return false;
}

bool GEMorphAnimationController::PauseActiveAnimation()
{
	if (m_pActiveAnimation != NULL)
	{
		m_pActiveAnimation->Pause();
		return true;
	}
	else
		return false;
}

bool GEMorphAnimationController::ResumeActiveAnimation()
{
	if (m_pActiveAnimation != NULL)
	{
		m_pActiveAnimation->Resume();
		return true;
	}
	else
		return false;
}


void GEMorphAnimationController::MakeXATXT2MA(const string& file)
{
	ifstream fin(file.c_str());
	string outfile = file.substr(0, file.length()-6);	// 去掉那个 xa.TXT
	outfile += "ma";
	ofstream fout(outfile.c_str(), ios::binary);	

	bool endofkey = false;

	// 解析
	while (!fin.eof())		
	{
		string s = GEUTIL::GetNextStrNotCmt(fin);
		if (s == "Mesh")	// get "Mesh XX {"
		{
			_MakeAniKeyXATxt2Binary(fin, fout);
			fout.write(reinterpret_cast<const char*>(&_EndKey), sizeof(int));	
		}
		else if (s == "Animation")
		{
			if (!endofkey)
			{
				endofkey = true;
				fout.seekp(-static_cast<int>(sizeof(int)), ios::cur);
				fout.write(reinterpret_cast<const char*>(&_EndKeys), sizeof(int));	
			}
			
			_MakeAniTxt2Binary(fin, fout);

			fout.write(reinterpret_cast<const char*>(&_EndAni), sizeof(int));	
		}
	}

	fout.seekp(-static_cast<int>(sizeof(int)), ios::cur);
	fout.write(reinterpret_cast<const char*>(&_EndFile), sizeof(int));	

	fin.close();
	fout.close();
}


void GEMorphAnimationController::_MakeAniKeyXATxt2Binary(ifstream& fin, ofstream& fout)
{
	string keyName = GEUTIL::GetNextStrNotCmt(fin);	// name
	GEUTIL::GetThisLineRest(fin);
    
	// 顶点数
    int nVertexs = GEUTIL::StrToInt_(GEUTIL::GetNextStrNotCmt(fin));

	float* pVertexs = new float[nVertexs*3];
	float* pf = pVertexs;
	for (int i = 0; i < nVertexs*3; ++i)
	{
		*(pf++) = GEUTIL::StrToFloat(GEUTIL::GetNextStrNotCmt(fin));
	}

	// 面信息
	int nFaces = GEUTIL::StrToInt_(GEUTIL::GetNextStrNotCmt(fin));
	char* pcFaces = new char[nFaces*sizeof(unsigned short)*3];
	unsigned short* pFaces = reinterpret_cast<unsigned short*>(pcFaces);
	for (int i = 0; i < nFaces; ++i)
	{
		GEUTIL::GetNextStrNotCmt(fin);	// 3
		unsigned short v1 = GEUTIL::StrToInt_(GEUTIL::GetNextStrNotCmt(fin));
		unsigned short v2 = GEUTIL::StrToInt_(GEUTIL::GetNextStrNotCmt(fin));
		unsigned short v3 = GEUTIL::StrToInt_(GEUTIL::GetNextStrNotCmt(fin));
		*(pFaces++) = v1;
		*(pFaces++) = v2;
		*(pFaces++) = v3;
	}
	pFaces = reinterpret_cast<unsigned short*>(pcFaces);


	// 关于法向
	while (true)		// get "MeshNormals {"
	{
		string s = GEUTIL::GetNextStrNotCmt(fin);
		if (s.length() >= 11 && s.substr(0, 11) == "MeshNormals")
		{
			GEUTIL::GetThisLineRest(fin);
			break;
		}
	}

	int nNormals = GEUTIL::StrToInt_(GEUTIL::GetNextStrNotCmt(fin));
	float* pNormals = new float[nNormals*3];
	pf = pNormals;
	for (int i = 0; i < nNormals; ++i)
	{
		float x = GEUTIL::StrToFloat(GEUTIL::GetNextStrNotCmt(fin));
		float y = GEUTIL::StrToFloat(GEUTIL::GetNextStrNotCmt(fin));
		float z = GEUTIL::StrToFloat(GEUTIL::GetNextStrNotCmt(fin));
		*(pf++) = x;
		*(pf++) = y;
		*(pf++) = z;
	}

	float* pVNs = new float[nVertexs*3];	// 顶点对应的法向
	pf = pVNs;
	GEUTIL::GetNextStrNotCmt(fin);			// m个面
	for (int i = 0; i < nFaces; ++i)
	{
		GEUTIL::GetNextStrNotCmt(fin);		// 3
		int nindex1 = GEUTIL::StrToInt_(GEUTIL::GetNextStrNotCmt(fin));	// 面三个顶点对应的法向index
		int nindex2 = GEUTIL::StrToInt_(GEUTIL::GetNextStrNotCmt(fin));
		int nindex3 = GEUTIL::StrToInt_(GEUTIL::GetNextStrNotCmt(fin));

		// 该面对应的顶点号
		int iVertex1 = pFaces[i*3+0];
		int iVertex2 = pFaces[i*3+1];
		int iVertex3 = pFaces[i*3+2];

		// 填充法向
		*(pf+iVertex1*3+0) = *(pNormals+nindex1*3+0);
		*(pf+iVertex1*3+1) = *(pNormals+nindex1*3+1);
		*(pf+iVertex1*3+2) = *(pNormals+nindex1*3+2);

		*(pf+iVertex2*3+0) = *(pNormals+nindex2*3+0);
		*(pf+iVertex2*3+1) = *(pNormals+nindex2*3+1);
		*(pf+iVertex2*3+2) = *(pNormals+nindex2*3+2);

		*(pf+iVertex2*3+0) = *(pNormals+nindex3*3+0);
		*(pf+iVertex2*3+1) = *(pNormals+nindex3*3+1);
		*(pf+iVertex2*3+2) = *(pNormals+nindex3*3+2);
	}

	// 写入
	_WriteBinaryStr(keyName, fout);													// key name
	fout.write(reinterpret_cast<const char*>(&nVertexs), sizeof(int));				// vertex count
	fout.write(reinterpret_cast<const char*>(pVertexs), sizeof(float)*3*nVertexs);	// vertexs
	fout.write(reinterpret_cast<const char*>(pVNs), sizeof(float)*3*nVertexs);		// normals

	delete [] pVertexs;
	delete [] pNormals;
	delete [] pcFaces;
	delete [] pVNs;
}

void GEMorphAnimationController::_MakeAniTxt2Binary(ifstream& fin, ofstream& fout)
{
	string aniName = GEUTIL::GetNextStrNotCmt(fin);	// name
	GEUTIL::GetThisLineRest(fin);					// {
	int nKeys = GEUTIL::StrToInt_(GEUTIL::GetNextStrNotCmt(fin));
	
	_WriteBinaryStr(aniName, fout);									// key name
	fout.write(reinterpret_cast<const char*>(&nKeys), sizeof(int));	// key count
	for (int i = 0; i < nKeys; ++i)
	{
		int timeKey = GEUTIL::StrToInt_(GEUTIL::GetNextStrNotCmt(fin));
		string keyName = GEUTIL::GetNextStrNotCmt(fin);
		
		fout.write(reinterpret_cast<const char*>(&timeKey), sizeof(int));	// time key
		_WriteBinaryStr(keyName, fout);										// key name
	}
}

void GEMorphAnimationController::_WriteBinaryStr(const string& str, ofstream& fout)
{
	int i = str.length();
	fout.write(reinterpret_cast<const char*>(&i), sizeof(int));	// length
	fout.write(str.c_str(), str.length());
}


void GEMorphAnimationController::MakeGEATXT2MA(const string& file)
{
	ifstream fin(file.c_str());
	string outfile = file.substr(0, file.length()-7);	// 去掉那个 gea.TXT
	outfile += "ma";
	ofstream fout(outfile.c_str(), ios::binary);

	bool endofkey = false;

	// 解析
	while (!fin.eof())		
	{
		string s = GEUTIL::GetNextStrNotCmt(fin);
		if (s == "Mesh")	// get "Mesh XX {"
		{
			_MakeAniGEAKeyTxt2Binary(fin, fout);
			fout.write(reinterpret_cast<const char*>(&_EndKey), sizeof(int));	
		}
		else if (s == "Animation")
		{
			if (!endofkey)
			{
				endofkey = true;
				fout.seekp(-static_cast<int>(sizeof(int)), ios::cur);
				fout.write(reinterpret_cast<const char*>(&_EndKeys), sizeof(int));	
			}

			_MakeAniTxt2Binary(fin, fout);

			fout.write(reinterpret_cast<const char*>(&_EndAni), sizeof(int));	
		}
	}

	fout.seekp(-static_cast<int>(sizeof(int)), ios::cur);
	fout.write(reinterpret_cast<const char*>(&_EndFile), sizeof(int));	

	fin.close();
	fout.close();	
}


void GEMorphAnimationController::_MakeAniGEAKeyTxt2Binary(ifstream& fin, ofstream& fout)
{
	string keyName = GEUTIL::GetNextStrNotCmt(fin);	// name
	GEUTIL::GetThisLineRest(fin);					// {

	// 顶点数
	int nVertexs = GEUTIL::StrToInt_(GEUTIL::GetNextStrNotCmt(fin));

	float* pVertexs = new float[nVertexs*3];
	float* pNormals = new float[nVertexs*3];

	float* pv = pVertexs;
	float* pn = pNormals;
	for (int i = 0; i < nVertexs; ++i)
	{
		// position
		*(pv++) = GEUTIL::StrToFloat(GEUTIL::GetNextStrNotCmt(fin));
		*(pv++) = GEUTIL::StrToFloat(GEUTIL::GetNextStrNotCmt(fin));
		*(pv++) = GEUTIL::StrToFloat(GEUTIL::GetNextStrNotCmt(fin));

		// normals
		*(pn++) = GEUTIL::StrToFloat(GEUTIL::GetNextStrNotCmt(fin));
		*(pn++) = GEUTIL::StrToFloat(GEUTIL::GetNextStrNotCmt(fin));
		*(pn++) = GEUTIL::StrToFloat(GEUTIL::GetNextStrNotCmt(fin));
	}


	// 写入
	_WriteBinaryStr(keyName, fout);													// key name
	fout.write(reinterpret_cast<const char*>(&nVertexs), sizeof(int));				// vertex count
	fout.write(reinterpret_cast<const char*>(pVertexs), sizeof(float)*3*nVertexs);	// vertexs
	fout.write(reinterpret_cast<const char*>(pNormals), sizeof(float)*3*nVertexs);	// normals

	delete [] pVertexs;
	delete [] pNormals;
}


void GEMorphAnimationController::LoadFromMA(const string& file)
{
	ifstream fin(file.c_str(), ios::binary);

	// suppose that m_AnimationSet is empty

	while(true)
	{
		string keyName = _ReadBinaryStr(fin);	// key name
		
		int nVertexs;
		fin.read(reinterpret_cast<char*>(&nVertexs), sizeof(int));
		
		char* pVertexs = new char[nVertexs*sizeof(float)*3];
		fin.read(pVertexs, nVertexs*sizeof(float)*3);

		char* pNormals = new char[nVertexs*sizeof(float)*3];
		fin.read(pNormals, nVertexs*sizeof(float)*3);
		
		GEMorphAnimationKey* key = new GEMorphAnimationKey(keyName, nVertexs);
		key->LoadFromSrc(reinterpret_cast<float*>(pVertexs), reinterpret_cast<float*>(pNormals));
		m_AnimationKeySet.insert(make_pair(keyName, key));

		delete [] pVertexs;
		delete [] pNormals;

		unsigned int sign;
		fin.read(reinterpret_cast<char*>(&sign), sizeof(int));
		if (sign == _EndKeys)
			break;	// begin load animation
	}

	while(true)
	{
		string aniName = _ReadBinaryStr(fin);	// ani name
		int nKeys;
		fin.read(reinterpret_cast<char*>(&nKeys), sizeof(int));

		m_AnimationSet[aniName].SetName(aniName);

		for (int i = 0; i < nKeys; ++i)
		{
			int iKeyTime;
			fin.read(reinterpret_cast<char*>(&iKeyTime), sizeof(int));
			float keyTime = iKeyTime/1000.f;
			string keyName = _ReadBinaryStr(fin);
			m_AnimationSet[aniName].AddKey(keyTime, m_AnimationKeySet[keyName]);
		}

		unsigned int sign;
		fin.read(reinterpret_cast<char*>(&sign), sizeof(int));
		if (sign == _EndFile)
			break;	// begin load animation
	}
}


string GEMorphAnimationController::_ReadBinaryStr(ifstream& fin)
{
	int i; 
	fin.read(reinterpret_cast<char*>(&i), sizeof(int));	// string length
	char* pStr = new char[i+1];
	fin.read(pStr, i);
	pStr[i] = '\0';
	string str = pStr;
	delete [] pStr;
	return str;
}

/*
GEMorphAnimationController* GEMorphAnimationControllerManager::CreateMorphAnimationController(const string& strName, const string& strOrigin)
{
	return (GEMorphAnimationController*)(g_MorphAnimationControllerManager.CreateResource(strName,strOrigin,new GEMorphAnimationController));
}

GEMorphAnimationController* GEMorphAnimationControllerManager::CreateMorphAnimationController(const string& strOrigin)
{
	return (GEMorphAnimationController*)(g_MorphAnimationControllerManager.CreateResource(strOrigin,new GEMorphAnimationController));
}
*/