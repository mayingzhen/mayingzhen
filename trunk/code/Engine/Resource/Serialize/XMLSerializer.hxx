#include "XMLSerializer.h"

namespace ma
{
	void XMLSerializer::Serialize(bool& val,const char* pszLable)
	{
		if ( IsReading() )
		{
			val = StringConverter::parseBool(m_pCurElem->Attribute(pszLable));
		}
		else
		{
			m_pCurElem->SetAttribute(pszLable,StringConverter::toString(val).c_str());
		}	
	}

// 	void XMLSerializer::Serialize(unsigned short& val, const char* pszLable)
// 	{
// 		if ( IsReading() )
// 		{
// 			val = StringConverter::parseInt(m_pCurElem->Attribute(pszLable));
// 		}
// 		else
// 		{
// 			m_pCurElem->SetAttribute(pszLable,StringConverter::toString(val).c_str());
// 		}
// 	}

	void XMLSerializer::Serialize(UINT& val,const char* pszLable)
	{
		if ( IsReading() )
		{
			val = StringConverter::parseUnsignedInt(m_pCurElem->Attribute(pszLable));
		}
		else
		{
			m_pCurElem->SetAttribute(pszLable,StringConverter::toString(val).c_str());
		}
	}

	void XMLSerializer::Serialize(int& val,const char* pszLable)
	{
		if ( IsReading() )
		{
			val = StringConverter::parseInt(m_pCurElem->Attribute(pszLable));
		}
		else
		{
			m_pCurElem->SetAttribute(pszLable,StringConverter::toString(val).c_str());
		}
	}

	void XMLSerializer::Serialize(unsigned short &val,const char* pszLable)
	{
		if ( IsReading() )
		{
			val = StringConverter::parseInt(m_pCurElem->Attribute(pszLable));
		}
		else
		{
			m_pCurElem->SetAttribute(pszLable,StringConverter::toString(val).c_str());
		}
	}

	void XMLSerializer::Serialize(long &val,const char* pszLable)
	{
		if ( IsReading() )
		{
			val = StringConverter::parseLong(m_pCurElem->Attribute(pszLable));
		}
		else
		{
			m_pCurElem->SetAttribute(pszLable,StringConverter::toString(val).c_str());
		}
	}

	void XMLSerializer::Serialize(Uint64& val,const char* pszLable)
	{
		ASSERT(false);
	}

	void XMLSerializer::Serialize(float& val,const char* pszLable)
	{
		if ( IsReading() )
		{
			val = StringConverter::parseReal(m_pCurElem->Attribute(pszLable));
		}
		else
		{
			m_pCurElem->SetAttribute(pszLable,StringConverter::toString(val).c_str());
		}		
	}

	void XMLSerializer::Serialize(std::string& val,const char* pszLable)
	{
		if ( IsReading() )
		{
			const char* pszValue = m_pCurElem->Attribute(pszLable);
			val = pszValue ? pszValue : "";
		}
		else
		{
			m_pCurElem->SetAttribute(pszLable,val.c_str());
		}
	}

	void XMLSerializer::Serialize(Vector2 &val,const char* pszLable)
	{
		Serialize(val.x,"x");
		Serialize(val.y,"y");
	}


	void XMLSerializer::Serialize(maGUID &val,const char* pszLable)
	{
		Serialize(val.m_a,"a");
		Serialize(val.m_b,"b");
	}

	void XMLSerializer::Serialize(Vector3 &val,const char* pszLable)
	{
		Serialize(val.x,"x");
		Serialize(val.y,"y");
		Serialize(val.z,"z");
	}


	void XMLSerializer::Serialize(Vector4 &val,const char* pszLable)
	{
		Serialize(val.x,"x");
		Serialize(val.y,"y");
		Serialize(val.z,"z");
		Serialize(val.w,"w");
	}


	void XMLSerializer::Serialize(Quaternion &val,const char* pszLable)
	{
		Serialize(val.x,"x");
		Serialize(val.y,"y");
		Serialize(val.z,"z");
		Serialize(val.w,"w");
	}

	void XMLSerializer::Serialize(Transform& val,const char *pszLable)
	{
		Serialize(val.m_vScale,"Scale");
		Serialize(val.m_qRot,"Rotation");
		Serialize(val.m_vPos,"Position");
	}

	void XMLSerializer::SerializeByte(Uint8* &pData,UINT nSizeInByte,const char* pszLable)
	{
		ASSERT(false);
	}
}

