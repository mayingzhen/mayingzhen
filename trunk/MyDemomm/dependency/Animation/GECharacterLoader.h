#pragma once
#include<string>
using namespace std;
class GECharacter;

/**
 * 骨架以及动画数据的解析器.
 * 扩展此类可以增加所支持的动画文件格式。
 */
class GECharacterLoader
{
public:
	GECharacterLoader(void);
	virtual ~GECharacterLoader(void);
	/**
	 * 解析OGRE的“.skeleton.xml” 文件
	 * @param cha 将使用解析的动画数据非得Character
	 * @param filename 要解析的文件名
	 */
	static bool parseSkeletonXML(GECharacter* cha, const string& filename);

	static bool parseMorphAnimation(GECharacter* cha, const string& filename);
};
