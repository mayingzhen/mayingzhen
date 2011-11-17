#include "GECharacterloader.h"
#include "GECharacter.h"
#include "GESkeleton.h"
#include "GEAnimationSet.h"
#include "GEUtil.h"
#include "GEStringParser.h"
#include "TinyXML/tinyxml.h"

GECharacterLoader::GECharacterLoader(void)
{
}

GECharacterLoader::~GECharacterLoader(void)
{
}


bool GECharacterLoader::parseSkeletonXML(GECharacter* cha, const string& filename)
{
	GESkeleton* skn = GESkeletonManager::GetSingleton().CreateSkeleton(filename);
	GEAnimationSet* aniset= GEAnimationSetManager::GetSingleton().CreateAnimationSet(filename);
	cha->GetAnimationController().BindSkeleton(skn);
	cha->GetAnimationController().BindAnimationSet(aniset);

	if (skn->GetTimesRef() > 1 && aniset->GetTimesRef() > 1)
		return true;

	TiXmlElement *element, *root;

	string file;
	GEUTIL::ReadWholeFile(filename,file);	
	TiXmlDocument* doc = new TiXmlDocument();

	doc->Parse(file.c_str());

	if(doc->Error())
	{
		delete doc;
		GELOGINFO("XML parsing error!, Parsing XML skeleton file failed.");
	}
    
	root=doc->RootElement();
	if( string(root->Value()) != "skeleton" ) 
	{
		delete doc;
		GELOGINFO("XML parsing error!, Parsing XML Mesh faild.");
	}

	// bones
	element=root->FirstChildElement("bones");
	if(element)
	{
		TiXmlElement* bone=element->FirstChildElement("bone");
		while(bone)
		{
			string id,name;  unsigned short sid;
			id=bone->Attribute("id");
			name=bone->Attribute("name");
			sid=static_cast<unsigned short>(GEStringParser::parseInt(id));

			TiXmlElement *pos, *rot;
			pos=bone->FirstChildElement("position");
			float x,y,z;
			x=GEStringParser::parseFloat(pos->Attribute("x"));
			// peter
			z=GEStringParser::parseFloat(pos->Attribute("y"));
			y=GEStringParser::parseFloat(pos->Attribute("z"));
            
			rot=bone->FirstChildElement("rotation");
			// peter, 变成负号，因为左右手坐标系中同向旋转但正负不同
			float angle = -GEStringParser::parseFloat(rot->Attribute("angle"));
			TiXmlElement *axis;
			axis=rot->FirstChildElement("axis");
			float a,b,c;
			a=GEStringParser::parseFloat(axis->Attribute("x"));
			// peter
			c=GEStringParser::parseFloat(axis->Attribute("y"));
			b=GEStringParser::parseFloat(axis->Attribute("z"));
            
			GEBone* gb=skn->createBone(sid,name);	// 父节点等一下再设
			Quaternion orii=Quaternion(angle,Vector3(a,b,c));
			gb->setInitialTransform(Vector3(x,y,z), orii);
            
			bone=bone->NextSiblingElement("bone");
		}
	}

	// bonehierarchy
	element = root->FirstChildElement("bonehierarchy");
	if(element)
	{
		TiXmlElement* pa=element->FirstChildElement("boneparent");
		while(pa)
		{
			string bone,parent;
			bone=pa->Attribute("bone");
			parent=pa->Attribute("parent");
			GEBone* b=skn->getBone(bone);
			GEBone* p=skn->getBone(parent);
			if(b && p) b->setParent( p->getIndex() );

			pa=pa->NextSiblingElement("boneparent");
		}
		skn->findRoots();	// 整理skeleton中根节点的信息
	}
	
	// animations
	element = root->FirstChildElement("animations");
	if(element)
	{
		TiXmlElement* xmlani=element->FirstChildElement("animation");
		while(xmlani)
		{
			static int index=1000; // 从1000算起
            string name;
			float length;
			name=xmlani->Attribute("name");
			length=GEStringParser::parseFloat(xmlani->Attribute("length"));            
			GEAnimation* ani = new GEAnimation(name, length, index++);
            
			TiXmlElement* tracks=xmlani->FirstChildElement("tracks");
			if(tracks)
			{
                TiXmlElement* xmltrack=tracks->FirstChildElement("track");
				while(xmltrack)
				{
					string bonename=xmltrack->Attribute("bone");
					GEAnimationTrack* track = new GEAnimationTrack( skn->getBone(bonename)->getIndex() );

					TiXmlElement* keyframes = xmltrack->FirstChildElement("keyframes");
					if(keyframes)
					{
						TiXmlElement* xmlkf=keyframes->FirstChildElement("keyframe");
						while(xmlkf)
						{
							GEKeyframe kf;
							float time=GEStringParser::parseFloat(xmlkf->Attribute("time"));
							kf.setTime(time);							

							// translate
							TiXmlElement* trans=xmlkf->FirstChildElement("translate");
                            float x,y,z;
							x=GEStringParser::parseFloat(trans->Attribute("x"));
							// peter
							z=GEStringParser::parseFloat(trans->Attribute("y"));
							y=GEStringParser::parseFloat(trans->Attribute("z"));
							kf.setTranslation(Vector3(x,y,z));

							// rotate
							TiXmlElement* rot=xmlkf->FirstChildElement("rotate");
							// peter,变成负号，因为左右手坐标系中同向旋转但正负不同
							float angle=-GEStringParser::parseFloat(rot->Attribute("angle"));
							TiXmlElement* xmlaxis=rot->FirstChildElement("axis");
							if(xmlaxis)
							{
								x=GEStringParser::parseFloat(xmlaxis->Attribute("x"));
								// peter
								z=GEStringParser::parseFloat(xmlaxis->Attribute("y"));
								y=GEStringParser::parseFloat(xmlaxis->Attribute("z"));
							}
							kf.setRotation(Quaternion(angle,Vector3(x,y,z)));
                            
							// scale
							TiXmlElement* scale=xmlkf->FirstChildElement("scale");
							if (scale)
							{
								x=GEStringParser::parseFloat(scale->Attribute("x"));
								// peter
								z=GEStringParser::parseFloat(scale->Attribute("y"));
								y=GEStringParser::parseFloat(scale->Attribute("z"));
								kf.setScale(Vector3(x,y,z));
							}
							else
								kf.setScale(Vector3(1,1,1));
                            
							track->addKeyframe(kf);
							xmlkf=xmlkf->NextSiblingElement("keyframe");
						}	// keyframe
					}	// keyframes
					ani->addTrack(bonename, track);
					xmltrack=xmltrack->NextSiblingElement("track");
				}	//track
			}	// tracks
			aniset->addAnimation(ani);
			xmlani = xmlani->NextSiblingElement("animation");
		}	// animation        
	}	// animations
	skn->computeInverseInitialTransform();

	//这里还没InitDeviceObjects()，所以还没有submesh，所以create shadow没有用
	//cha->createShadowVB();

	return true;
}


