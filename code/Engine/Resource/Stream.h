#ifndef _Stream_H_
#define _Stream_H_

namespace ma
{
	class Stream : public Referenced
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
		
		/// Read a 32-bit integer.
		int ReadInt();
		/// Read a 16-bit integer.
		short ReadShort();
		/// Read an 8-bit integer.
		signed char ReadByte();
		/// Read a 32-bit unsigned integer.
		unsigned ReadUInt();
		/// Read a 16-bit unsigned integer.
		unsigned short ReadUShort();
		/// Read an 8-bit unsigned integer.
		unsigned char ReadUByte();
		/// Read a bool.
		bool ReadBool();
		/// Read a float.
		float ReadFloat();
		/// Read an IntRect.
		//IntRect ReadIntRect();
		/// Read an IntVector2.
		//IntVector2 ReadIntVector2();
		/// Read a Rect.
		//Rect ReadRect();
		/// Read a Vector2.
		Vector2 ReadVector2();
		/// Read a Vector3.
		Vector3 ReadVector3();
		/// Read a Vector3 packed into 3 x 16 bits with the specified maximum absolute range.
		Vector3 ReadPackedVector3(float maxAbsCoord);
		/// Read a Vector4.
		Vector4 ReadVector4();
		/// Read a quaternion.
		Quaternion ReadQuaternion();
		/// Read a quaternion with each component packed in 16 bits.
		//Quaternion ReadPackedQuaternion();
		/// Read a Matrix3.
		//Matrix3 ReadMatrix3();
		/// Read a Matrix3x4.
		Matrix3x4 ReadMatrix3x4();
		/// Read a Matrix4.
		Matrix4 ReadMatrix4();
		Transform ReadTransform();
		/// Read a color.
		//ColourValue ReadColor();
		/// Read a bounding box.
		AABB ReadBoundingBox();
		AABB2D ReadBoundingBox2D();
		/// Read a null-terminated string.

		inline string ReadString();

		/// Write a 32-bit integer.
		bool WriteInt(int value);
		/// Write a 16-bit integer.
		bool WriteShort(short value);
		/// Write an 8-bit integer.
		bool WriteByte(signed char value);
		/// Write a 32-bit unsigned integer.
		bool WriteUInt(unsigned value);
		/// Write a 16-bit unsigned integer.
		bool WriteUShort(unsigned short value);
		/// Write an 8-bit unsigned integer.
		bool WriteUByte(unsigned char value);
		/// Write a bool.
		bool WriteBool(bool value);
		/// Write a float.
		bool WriteFloat(float value);
		/// Write an IntRect.
		//bool WriteIntRect(const IntRect& value);
		/// Write an IntVector2.
		//bool WriteIntVector2(const IntVector2& value);
		/// Write a Rect.
		//bool WriteRect(const Rect& value);
		/// Write a Vector2.
		bool WriteVector2(const Vector2& value);
		/// Write a Vector3.
		bool WriteVector3(const Vector3& value);
		/// Write a Vector3 packed into 3 x 16 bits with the specified maximum absolute range.
		//bool WritePackedVector3(const Vector3& value, float maxAbsCoord);
		/// Write a Vector4.
		bool WriteVector4(const Vector4& value);
		/// Write a quaternion.
		bool WriteQuaternion(const Quaternion& value);
		/// Write a quaternion with each component packed in 16 bits.
		//bool WritePackedQuaternion(const Quaternion& value);
		/// Write a Matrix3.
		bool WriteMatrix3(const Matrix3& value);
		/// Write a Matrix3x4.
		bool WriteMatrix3x4(const Matrix3x4& value);
		/// Write a Matrix4.
		bool WriteMatrix4(const Matrix4& value);
		/// Write a color.
		//bool WriteColor(const Color& value);
		/// Write a bounding box.
		bool WriteBoundingBox(const AABB& value);
		bool WriteBoundingBox2D(const AABB2D& value);
		/// Write a null-terminated string.
		inline bool WriteString(const string& value);

	protected:
		UINT			m_nSize;
		uint16			m_eAccessMode;
		std::string		m_strName;

		#define OGRE_STREAM_TEMP_SIZE 128

	};

	string Stream::ReadString()
	{
		string ret;

		while (!this->Eof())
		{
			char c = this->ReadByte();
			if (!c)
				break;
			else
				ret += c;
		}

		return ret;
	}

	bool Stream::WriteString(const string& value)
	{
		const char* chars = value.c_str();
		// Count length to the first zero, because ReadString() does the same
		unsigned length = value.length();
		return Write(chars, length + 1) == length + 1;
	}

}

#endif
