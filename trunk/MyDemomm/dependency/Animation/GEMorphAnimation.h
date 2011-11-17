#ifndef GEMORPHANIMATION_H
#define GEMORPHANIMATION_H

//#include "vector.h"
//#include "GEResource.h"
//#include "GEResourceManager.h"
#include <vector>
#include <map>
using namespace std;

#include <d3dx9.h>

/** Morph Animation �Ĺؼ�֡ */
class GEMorphAnimationKey
{
public :
	GEMorphAnimationKey(const string& name, unsigned int nVertexs);
	~GEMorphAnimationKey();

	inline const string&  GetName()	{ return m_sName; }

	inline const D3DXVECTOR3& GetPosition(int index) const	{ return m_vPositions[index];	}
	inline const D3DXVECTOR3& GetNormal(int index) const	{ return m_vNormals[index];		}

	inline unsigned int GetVertexCount() const			{ return m_nVertexs;			}

	inline void SetPosition(unsigned int index, float x, float y, float z)
	{
		m_vPositions[index].x = x;
		m_vPositions[index].y = y;
		m_vPositions[index].z = z;
	}

	inline void SetNormal(unsigned int index, float x, float y, float z)
	{
		m_vNormals[index].x = x;
		m_vNormals[index].y = y;
		m_vNormals[index].z = z;
	}

	bool LoadFromSrc(float* pPositionSrc, float* pNormalSrc);

	void PrepareMemory();

private:
	string	m_sName;
	unsigned int m_nVertexs;			

	D3DXVECTOR3*	m_vPositions;
	D3DXVECTOR3*	m_vNormals;
};




/** һ��Morph Animation����������ؼ�֡ */
class GEMorphAnimation
{
	friend class GEMorphAnimationController;
public:
	typedef pair<float, GEMorphAnimationKey*> GEMorphAnimationPair;

	GEMorphAnimation();
	~GEMorphAnimation();

	inline const string&  GetName()	{ return m_sName; }
	inline void SetName(const string& name) { m_sName = name; }
	
	bool AddKey(float time, GEMorphAnimationKey* key);	

	bool Update(float deltaTime);
 
	/** ���ƺ���Ⱥ */
	void Play(bool loop, float startTime = 0);
	bool SetTimeScale(float timeScale);
	void Pause();
	/** @note: call must after Pause() */
	void Resume();

	/** �õ���ǰ֡λ������������ʲôλ��
	 *	@return [float] 0��1
	 */
	float GetCurrentPoseKeyPartition() const;

	/** �����Ƿ���ĩβ֡ */
	bool IsEnd() const;

	/** �õ��������ŵĳ��� */
	float GetAnimationLength() const;

	/** �õ���ǰ���ŵ�λ�� */
	float GetCurrentTime() const;
	
	GEMorphAnimationKey* GetCurrentPoseKey() const { return m_CurrentPose.second; }

private:
	vector< GEMorphAnimationPair > m_Keys;
	
	string	m_sName;
	
	bool	m_bPlaying;
	bool	m_bLoop;
	float	m_fTimeScale;

	vector< GEMorphAnimationPair >::iterator m_SourceKey;
	vector< GEMorphAnimationPair >::iterator m_TargetKey;
	GEMorphAnimationPair m_CurrentPose;
};




/** һ����ɫ�� Morph Animation ����*/
class GEMorphAnimationController
{
public:
	GEMorphAnimationController();
	~GEMorphAnimationController();

	unsigned int AddAnimation(GEMorphAnimation ani);
	
	/** return if need update mesh */
	bool Update(float deltaTime);

	bool PlayAnimation(const string& name, bool loop, float startTime = 0);
	bool StopActiveAnimation();
	bool PauseActiveAnimation();
	bool ResumeActiveAnimation();

	inline GEMorphAnimation* GetActiveAnimation() const { return m_pActiveAnimation; }
	inline GEMorphAnimation* GetActiveAnimation()		{ return m_pActiveAnimation; }

	/** ��X�ļ�ת�ɶ������ļ� */
	static void MakeXATXT2MA(const string& file);	
	/** �Զ���GEA.TXT�ļ�ת�ɶ������ļ� */
	static void MakeGEATXT2MA(const string& file);

	void LoadFromMA(const string& file);
	
	inline unsigned short GetAnimationCount() const { return static_cast<unsigned short>(m_AnimationSet.size()); }

private:
	map<string, GEMorphAnimationKey*>	m_AnimationKeySet;
	map<string, GEMorphAnimation>		m_AnimationSet;
	GEMorphAnimation*					m_pActiveAnimation;

	static void _MakeAniKeyXATxt2Binary(ifstream& fin, ofstream& fout);
	static void _MakeAniTxt2Binary(ifstream& fin, ofstream& fout);
	static void _MakeAniGEAKeyTxt2Binary(ifstream& fin, ofstream& fout);
	static void _WriteBinaryStr(const string& str, ofstream& fout);

	string _ReadBinaryStr(ifstream& fin);

	static const unsigned int _EndKey	= 0xFFFFFA;	// ����Key���
	static const unsigned int _EndKeys	= 0xFFFFFB;	// ����Key���
	static const unsigned int _EndAni	= 0xFFFFFC;	// ����Ani���
	static const unsigned int _EndFile	= 0xFFFFFF;	// �ļ����

};

/*
class GEMorphAnimationControllerManager : public GEResourceManager
{
public:
	~GEMorphAnimationControllerManager(void){};

	static GEMorphAnimationControllerManager& GetSingleton()
	{ return g_MorphAnimationControllerManager; }

	GEMorphAnimationController* CreateMorphAnimationController(const string& strName, const string& strOrigin);
	GEMorphAnimationController* CreateMorphAnimationController(const string& strOrigin);

	// �õ�һ��AnimationSet
	inline GEMorphAnimationController* GetMorphAnimationController(ResHandle h)
	{ return ((GEMorphAnimationController *)g_MorphAnimationControllerManager.GetResource(h)); }

	inline GEMorphAnimationController* GetMorphAnimationController(const string& name)
	{ return ((GEMorphAnimationController *)g_MorphAnimationControllerManager.GetResource(name)); }


private:
	GEMorphAnimationControllerManager() {}

	static GEMorphAnimationControllerManager g_MorphAnimationControllerManager;
};
*/

#endif