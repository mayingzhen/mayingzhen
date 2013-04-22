#ifndef __Property_H__
#define __Property_H__

namespace ma
{
	enum PropertyRefMode
	{
		RefMode_Composite = 0,
		RefMode_Aggregate,		
	};

	class COMMON_API MetaProperty
	{
		PropertyRefMode m_refMode;
		bool			m_bTransient;

	};

	enum PropertyType
	{
		PropType_bool = 0,
		PropType_char,
		PropType_short,
		PropType_int,
		PropType_long,
		PropType_uchar,
		PropType_ushort,
		PropType_uint,
		PropType_ulong,
		PropType_float,
		PropType_string,
		PropType_name,
		PropType_struct,
		PropType_num,
	};

	inline Uint GetPropertySize(PropertyType propType)
	{
		static Uint propSize[PropType_num] = {
			sizeof(bool)
			,sizeof(char)
			,sizeof(short)
			,sizeof(int)
			,sizeof(long)
			,sizeof(unsigned char)
			,sizeof(unsigned short)
			,sizeof(unsigned int)
			,sizeof(unsigned long)
			,sizeof(float)
			,0
			,0
			,0
		};

		return propSize[propType];
	}

	class COMMON_API PropertyVisitor
	{
	public:


		virtual void VisitProperty(bool& val,const char* propName,bool defVal = false);

		virtual void VisitProperty(char& val,const char* propName,char defVal = 0);

		virtual void VisitProperty(short& val,const char* propName,short defVal = 0);

		virtual void VisitProperty(int& val,const char* propName,int defVal = 0);

		virtual void VisitProperty(long& val,const char* propName,long defVal = 0);

		virtual void VisitProperty(unsigned char& val,const char* propName,unsigned char defVal = 0);

		virtual void VisitProperty(unsigned short& val,const char* propName,unsigned short defVal = 0);

		virtual void VisitProperty(unsigned int& val,const char* propName,unsigned int defVal = 0);

		virtual void VisitProperty(unsigned long& val,const char* propName,unsigned long defVal = 0);

		virtual void VisitProperty(float& val,const char* propName, float defVal = 0.0f);

		virtual void VisitProperty(std::string& val,const char* propName, const char* defVal = "");

		virtual void VisitProperty(NodeTransform& val,const char* propName);

		virtual void VisitProperty(Object*& val,const char* propName,PropertyRefMode refMode);

		virtual bool IsLoad();

		virtual void BeginVisitGroup(const char* propName,const char* groupType);

		virtual void EndVisiteGroup(const char* propName);



		template<class ArrayType>
		void VisitHObjectArrayProperty(ArrayType& val,const char* propName,PropertyRefMode refMode)
		{
			//Travel Array property

			this->BeginVisitGroup(propName,"ObjectHandleArray");

			Uint nSize = val.size();
			this->VisitProperty(nSize,"size");

			if (this->IsLoad())
			{
				val.resize(nSize);
			}

			char buf[256];

			typedef typename ArrayType::value_type HandleType;
			typedef typename HandleType::value_type ElementType;

			for (Uint nCnt = 0; nCnt < nSize; ++nCnt)
			{
				sprintf(buf,"Elem_%d",nCnt);
				Object* pObj = val[nCnt].GetData();
				this->VisitProperty((Object*&)pObj,buf,refMode);
				val[nCnt] = SafeCast<ElementType>(pObj);
			}

			this->EndVisiteGroup(propName);
		}

		/*!\brief
		e.g. 
		std::vector<int> 
		std::vector<S3AObject*>
		S3AArray<int>

		but not std::vector<int*>
		*/
		template<class ArrayType>
		void VisitObjectArrayProperty(ArrayType& val,const char* propName,PropertyRefMode refMode)
		{
			//Travel Array property

			this->BeginVisitGroup(propName,"ObjectArray");

			Uint nSize = val.size();
			this->VisitProperty(nSize,"size");

			if (this->IsLoad())
			{
				val.resize(nSize);
			}

			char buf[256];

			for (Uint nCnt = 0; nCnt < nSize; ++nCnt)
			{
				sprintf(buf,"Elem_%d",nCnt);
				this->VisitProperty((Object*&)val[nCnt],buf,refMode);
			}

			this->EndVisiteGroup(propName);
		}


		template<class ArrayType>
		void VisitArrayProperty(ArrayType& val,const char* propName,PropertyRefMode refMode)
		{
			//Travel Array property

			this->BeginVisitGroup(propName,"Array");

			Uint nSize = val.size();
			this->VisitProperty(nSize,"size");

			if (this->IsLoad())
			{
				val.resize(nSize);
			}

			char buf[256];

			for (Uint nCnt = 0; nCnt < nSize; ++nCnt)
			{
				sprintf(buf,"Elem_%d",nCnt);
				this->VisitProperty(val[nCnt],buf,refMode);
			}

			this->EndVisiteGroup(propName);
		}

		//template<class MapType>
		//void VisitMapProperty(ArrayType& val,const char* propName,PropertyRefMode refMode)
		//{
		//	this->BeginVisitGroup(propName);
		//	

		//	typename MapType::iterator iter =

		//	this->EndVisiteGroup();
		//}

	};

}





#endif //__Property_H__