#ifndef __XMLSerialize_H__
#define __XMLSerialize_H__


namespace ma
{

	class ENGINE_API XMLSaveVisitor : public PropertyVisitor
	{
		TiXmlElement* m_pRootElem;
		TiXmlElement* m_pParentElem;

		std::vector<TiXmlElement*> m_arrGroupStack;

	public:

		//void SaveExternLink(TiXmlElement* pParent,ObjectVec& objVec);

		void SaveToXML(TiXmlElement* pParent,Object* pObj,const char* propName);


		virtual void BeginVisitGroup(const char* propName,const char* groupType);

		virtual void EndVisiteGroup(const char* propName);

		virtual void VisitProperty(bool& val,const char* propName,bool defVal);

		virtual void VisitProperty(char& val,const char* propName,char defVal);

		virtual void VisitProperty(short& val,const char* propName,short defVal);

		virtual void VisitProperty(int& val,const char* propName,int defVal);

		virtual void VisitProperty(long& val,const char* propName,long defVal);

		virtual void VisitProperty(unsigned char& val,const char* propName,unsigned char defVal);

		virtual void VisitProperty(unsigned short& val,const char* propName,unsigned short defVal);

		virtual void VisitProperty(unsigned int& val,const char* propName,unsigned int defVal);

		virtual void VisitProperty(unsigned long& val,const char* propName,unsigned long defVal);

		virtual void VisitProperty(float& val,const char* propName, float defVal);

		virtual void VisitProperty(std::string& val,const char* propName, const char* defVal);

		virtual void VisitProperty(const char*& val,const char* propName,const char* defVal);

		virtual void VisitProperty(Object*& val,const char* propName,PropertyRefMode refMode);
	};
}

#endif 