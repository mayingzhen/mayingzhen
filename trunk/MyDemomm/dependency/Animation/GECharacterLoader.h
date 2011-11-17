#pragma once
#include<string>
using namespace std;
class GECharacter;

/**
 * �Ǽ��Լ��������ݵĽ�����.
 * ��չ�������������֧�ֵĶ����ļ���ʽ��
 */
class GECharacterLoader
{
public:
	GECharacterLoader(void);
	virtual ~GECharacterLoader(void);
	/**
	 * ����OGRE�ġ�.skeleton.xml�� �ļ�
	 * @param cha ��ʹ�ý����Ķ������ݷǵ�Character
	 * @param filename Ҫ�������ļ���
	 */
	static bool parseSkeletonXML(GECharacter* cha, const string& filename);

	static bool parseMorphAnimation(GECharacter* cha, const string& filename);
};
