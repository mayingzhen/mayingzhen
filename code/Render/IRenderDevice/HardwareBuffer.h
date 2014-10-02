#ifndef  _HardwareBuffer__H__
#define  _HardwareBuffer__H__

namespace ma
{
	class HardwareBuffer : public Referenced
	{
	public:
		HardwareBuffer()
		{
			m_Stride = 0;
			m_Size = 0;
			m_Usage = USAGE_NO;
			m_pData = NULL;
		}

		virtual ~HardwareBuffer()
		{
			//SAFE_DELETE(m_pData);
		}

		virtual void*	Lock(int iOffsetBytes, int iLockSize, LOCK LockFlags) = 0;

		virtual void    Unlock() = 0;

		virtual void	Active() = 0;

		int             GetSize() const; 

		USAGE           GetUsage() const; 

		int				GetStride() const; 

		UINT			GetNumber() const;

		void*			GetData() const;

		void			SetData(void* pData,UINT nSize,int nStride,USAGE eUsage = USAGE_NO);

		virtual void	Serialize(Serializer& sl, const char* pszLable = "HardwareBuffer");

	protected:
		int				m_Stride;
		UINT            m_Size;
		USAGE           m_Usage;
		void*			m_pData;
	};

	inline void	HardwareBuffer::SetData(void* pData,UINT nSize,int nStride,USAGE eUsage)
	{
		m_pData = pData;
		m_Size = nSize;
		m_Stride = nStride;
		m_Usage = eUsage;
	}

	inline UINT HardwareBuffer::GetNumber() const
	{
		return m_Size / m_Stride;
	}
	
	inline int HardwareBuffer::GetSize() const 
	{ 
		return m_Size; 
	}

	inline USAGE HardwareBuffer::GetUsage() const 
	{
		return m_Usage; 
	}

	inline int HardwareBuffer::GetStride() const 
	{
		return m_Stride;
	}

	inline void* HardwareBuffer::GetData() const
	{
		return m_pData;
	}

	inline void	HardwareBuffer::Serialize(Serializer& sl, const char* pszLable/* = "HardwareBuffer"*/)
	{
		sl.BeginSection(pszLable);

		Uint8* pTemp = (Uint8*)m_pData; 

		sl.Serialize(m_Stride,"Stride");
		sl.Serialize(m_Size,"Size");
		sl.SerializeByte(pTemp,m_Size,"Data");

		m_pData = pTemp;

		sl.EndSection();
	}
}

#endif
