#ifndef _Stream_H_
#define _Stream_H_

namespace ma
{
	class ENGINE_API Stream : public Referenced
	{
	public:
		enum AccessMode
		{
			AM_READ = (1<<0), 
			AM_WRITE = (1<<1),
		};


	public:
		Stream(uint16 eAccessMode = AM_READ)
		{
			m_nSize = 0;
			m_eAccessMode = eAccessMode;
			m_strName = "";
		}

		Stream(const char* pszName, uint16 eAccessMode = AM_READ)
		{
			m_nSize = 0;
			m_eAccessMode = eAccessMode;
			m_strName = pszName ? pszName : "";
		}

		virtual ~Stream(){}


	public:
		// ---------------------------------------------------------------------
		// Get Attribute
		// ---------------------------------------------------------------------
		const char*		GetName() const{return m_strName.c_str();}
		uint16			GetAccessMode() const{return m_eAccessMode;}
		UINT			GetSize() const{return m_nSize;}

		bool			IsReadable() const{return (m_eAccessMode&AM_READ) != 0;}
		bool			IsWritable() const{return (m_eAccessMode&AM_WRITE) != 0;}


		// Read the requisite number of bytes from the stream, stopping at the end of the file.
		virtual UINT	Read(IN OUT void* pBuffer, UINT nCount) = 0;

		// Write the requisite number of bytes from the stream (only applicable to streams that are not read-only)
		virtual UINT	Write(const void* pBuffer, UINT nCount) = 0;

		/** Get a single line from the stream.
		@remarks
			The delimiter character is not included in the data
			returned, and it is skipped over so the next read will occur
			after it. The buffer contents will include a
			terminating character.
		@note
			If you used this function, you <b>must</b> open the stream in <b>binary mode</b>,
			otherwise, it'll produce unexpected results.
		@param buf Reference to a buffer pointer
		@param maxCount The maximum length of data to be read, excluding the terminating character
		@param delim The delimiter to stop at
		@returns The number of bytes read, excluding the terminating character
		*/
		virtual UINT ReadLine(char* buf, UINT maxCount, const std::string& delim = "\n");

		/** Skip a defined number of bytes. This can also be a negative value, in which case
		the file pointer rewinds a defined number of bytes*/
		virtual void Skip(int nCount) = 0;

		// Repositions the read point to a specified byte.
		virtual void Seek(UINT nPos) = 0;

		// Returns the current byte offset from beginning
		virtual UINT Tell() const = 0;

		// Returns true if the stream has reached the end.
		virtual bool Eof() const = 0;

		// Close the stream; this makes further operations invalid.
		virtual void Close() = 0;


	protected:
		UINT			m_nSize;
		uint16			m_eAccessMode;
		std::string		m_strName;

		#define OGRE_STREAM_TEMP_SIZE 128

	};

	DeclareRefPtr(Stream);
}

#endif
