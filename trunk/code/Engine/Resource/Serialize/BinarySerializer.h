#ifndef __BinarySerializer_H__
#define __BinarySerializer_H__

#include "Serializer.h"

namespace ma
{
	class BinarySerializer : public Serializer
	{
	public:
		virtual void	Serialize(bool& val,const char* pszLable);

		virtual void	Serialize(UINT& val,const char* pszLable);

		virtual void	Serialize(int& val,const char* pszLable);

		virtual void	Serialize(unsigned short& val,const char* pszLable);

		virtual void	Serialize(long& val,const char* pszLable);

		virtual void	Serialize(uint64& val,const char* pszLable);

		virtual void	Serialize(float& val,const char* pszLable);

		virtual void	Serialize(std::string& val,const char* pszLable);

		virtual void	Serialize(Vector2 &val,const char* pszLable);

		virtual void	Serialize(maGUID &val,const char* pszLable);

		virtual void	Serialize(Vector3 &val,const char* pszLable);

		virtual void	Serialize(Vector4 &val,const char* pszLable);

		virtual void	Serialize(Quaternion &val,const char* pszLable);

		virtual void	Serialize(Transform& val,const char *pszLable);
	
		virtual void	BeginSection(const char* pszLable);
	
		virtual void	EndSection();

	protected:
		StreamPtr		m_pStream;
	};
}
	
#endif// __BinarySerializer_H__