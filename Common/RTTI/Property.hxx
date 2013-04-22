#include "Property.h"


namespace ma
{
	bool PropertyVisitor::IsLoad()
	{
		return false;
	}

	void PropertyVisitor::BeginVisitGroup(const char* propName,const char* groupType)
	{
	}


	void PropertyVisitor::EndVisiteGroup(const char* propName)
	{
	}


	void PropertyVisitor::VisitProperty(bool& val,const char* propName,bool defVal)
	{

	}


	void PropertyVisitor::VisitProperty(char& val,const char* propName,char defVal)
	{

	}

	void PropertyVisitor::VisitProperty(short& val,const char* propName,short defVal)
	{
	}


	void PropertyVisitor::VisitProperty(int& val,const char* propName,int defVal)
	{

	}

	void PropertyVisitor::VisitProperty(long& val,const char* propName,long defVal)
	{

	}


	void PropertyVisitor::VisitProperty(unsigned char& val,const char* propName,unsigned char defVal)
	{
	}

	void PropertyVisitor::VisitProperty(unsigned short& val,const char* propName,unsigned short defVal)
	{
	}


	void PropertyVisitor::VisitProperty(unsigned int& val,const char* propName,unsigned int defVal)
	{

	}

	void PropertyVisitor::VisitProperty(unsigned long& val,const char* propName,unsigned long defVal)
	{

	}

	void PropertyVisitor::VisitProperty(float& val,const char* propName, float defVal )
	{
	}


	void PropertyVisitor::VisitProperty(std::string& val,const char* propName, const char* defVal)
	{
	}

	void PropertyVisitor::VisitProperty(NodeTransform& val,const char* propName)
	{

	}

	void PropertyVisitor::VisitProperty(Object*& val,const char* propName,PropertyRefMode refMode)
	{
		if (NULL != val)
		{
			val->TravelProperty(this);
		}
		
	}



}