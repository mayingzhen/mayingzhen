#include "Stream.h"

namespace ma
{
	UINT Stream::ReadLine(char* buf, UINT maxCount, const std::string& delim/* = "\n"*/)
	{
		// Deal with both Unix & Windows LFs
		bool trimCR = false;
		if (delim.find_first_of('\n') != std::string::npos)
		{
			trimCR = true;
		}

		char tmpBuf[128];
		size_t chunkSize = Math::Min(maxCount, (UINT)128-1);
		size_t totalCount = 0;
		size_t readCount; 
		while (chunkSize && (readCount = Read(tmpBuf, chunkSize)) != 0)
		{
			// Terminate
			tmpBuf[readCount] = '\0';

			// Find first delimiter
			size_t pos = strcspn(tmpBuf, delim.c_str());

			if (pos < readCount)
			{
				// Found terminator, reposition backwards
				Skip((long)(pos + 1 - readCount));
			}

			// Are we genuinely copying?
			if (buf)
			{
				memcpy(buf+totalCount, tmpBuf, pos);
			}
			totalCount += pos;

			if (pos < readCount)
			{
				// Trim off trailing CR if this was a CR/LF entry
				if (trimCR && totalCount && buf && buf[totalCount-1] == '\r')
				{
					--totalCount;
				}

				// Found terminator, break out
				break;
			}

			// Adjust chunkSize for next time
			chunkSize = Math::Min(maxCount-totalCount, (size_t)128-1);
		}

		// Terminate
		if(buf)
			buf[totalCount] = '\0';

		return totalCount;
	}


	int Stream::ReadInt()
	{
		int ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	short Stream::ReadShort()
	{
		short ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	signed char Stream::ReadByte()
	{
		signed char ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	unsigned Stream::ReadUInt()
	{
		unsigned ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	unsigned short Stream::ReadUShort()
	{
		unsigned short ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	unsigned char Stream::ReadUByte()
	{
		unsigned char ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	bool Stream::ReadBool()
	{
		if (ReadUByte())
			return true;
		else
			return false;
	}

	float Stream::ReadFloat()
	{
		float ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	// 	IntRect Stream::ReadIntRect()
	// 	{
	// 		int data[4];
	// 		Read(data, sizeof data);
	// 		return IntRect(data);
	// 	}
	// 
	// 	IntVector2 Stream::ReadIntVector2()
	// 	{
	// 		int data[2];
	// 		Read(data, sizeof data);
	// 		return IntVector2(data);
	// 	}

// 	Rect Stream::ReadRect()
// 	{
// 		float data[4];
// 		Read(data, sizeof data);
// 		return Rect(data);
// 	}

	Vector2 Stream::ReadVector2()
	{
		float data[2];
		Read(data, sizeof data);
		return Vector2(data);
	}

	Vector3 Stream::ReadVector3()
	{
		float data[3];
		Read(data, sizeof data);
		return Vector3(data);
	}

	Vector3 Stream::ReadPackedVector3(float maxAbsCoord)
	{
		float invV = maxAbsCoord / 32767.0f;
		short coords[3];
		Read(coords, sizeof coords);
		Vector3 ret(coords[0] * invV, coords[1] * invV, coords[2] * invV);
		return ret;
	}

	Vector4 Stream::ReadVector4()
	{
		float data[4];
		Read(data, sizeof data);
		return Vector4(data);
	}

	Quaternion Stream::ReadQuaternion()
	{
		float data[4];
		Read(data, sizeof data);
		return Quaternion(data);
	}

	// 	Quaternion Stream::ReadPackedQuaternion()
	// 	{
	// 		short coords[4];
	// 		Read(coords, sizeof coords);
	// 		Quaternion ret(coords[0] * invQ, coords[1] * invQ, coords[2] * invQ, coords[3] * invQ);
	// 		ret.Normalize();
	// 		return ret;
	// 	}

// 	Matrix3 Stream::ReadMatrix3()
// 	{
// 		float data[9];
// 		Read(data, sizeof data);
// 		return Matrix3(data);
// 	}

	Matrix3x4 Stream::ReadMatrix3x4()
	{
		float data[12];
		Read(data, sizeof data);
		return Matrix3x4(data);
	}

	Matrix4 Stream::ReadMatrix4()
	{
		float data[16];
		Read(data, sizeof data);
		return Matrix4(data);
	}

	Transform Stream::ReadTransform()
	{
		Transform tsf;	
		tsf.m_vPos = ReadVector3();
		tsf.m_qRot = ReadQuaternion();
		tsf.m_vScale = ReadVector3();
		return tsf;
	}

// 	ColourValue Stream::ReadColor()
// 	{
// 		float data[4];
// 		Read(data, sizeof data);
// 		return Color(data);
// 	}

	AABB Stream::ReadBoundingBox()
	{
		float data[6];
		Read(data, sizeof data);
		return AABB(Vector3(&data[0]), Vector3(&data[3]));
	}


	bool Stream::WriteInt(int value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Stream::WriteShort(short value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Stream::WriteByte(signed char value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Stream::WriteUInt(unsigned value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Stream::WriteUShort(unsigned short value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Stream::WriteUByte(unsigned char value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Stream::WriteBool(bool value)
	{
		return WriteUByte(value ? 1 : 0) == 1;
	}

	bool Stream::WriteFloat(float value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

// 	bool Stream::WriteIntRect(const IntRect& value)
// 	{
// 		return Write(value.Data(), sizeof value) == sizeof value;
// 	}
// 
// 	bool Stream::WriteIntVector2(const IntVector2& value)
// 	{
// 		return Write(value.Data(), sizeof value) == sizeof value;
// 	}
// 
// 	bool Stream::WriteRect(const Rect& value)
// 	{
// 		return Write(value.Data(), sizeof value) == sizeof value;
// 	}

	bool Stream::WriteVector2(const Vector2& value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Stream::WriteVector3(const Vector3& value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

// 	bool Stream::WritePackedVector3(const Vector3& value, float maxAbsCoord)
// 	{
// 		short coords[3];
// 		float v = 32767.0f / maxAbsCoord;
// 
// 		coords[0] = (short)(Clamp(value.x_, -maxAbsCoord, maxAbsCoord) * v + 0.5f);
// 		coords[1] = (short)(Clamp(value.y_, -maxAbsCoord, maxAbsCoord) * v + 0.5f);
// 		coords[2] = (short)(Clamp(value.z_, -maxAbsCoord, maxAbsCoord) * v + 0.5f);
// 		return Write(&coords[0], sizeof coords) == sizeof coords;
// 	}

	bool Stream::WriteVector4(const Vector4& value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Stream::WriteQuaternion(const Quaternion& value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

// 	bool Serializer::WritePackedQuaternion(const Quaternion& value)
// 	{
// 		short coords[4];
// 		Quaternion norm = value.Normalized();
// 
// 		coords[0] = (short)(Clamp(norm.w_, -1.0f, 1.0f) * q + 0.5f);
// 		coords[1] = (short)(Clamp(norm.x_, -1.0f, 1.0f) * q + 0.5f);
// 		coords[2] = (short)(Clamp(norm.y_, -1.0f, 1.0f) * q + 0.5f);
// 		coords[3] = (short)(Clamp(norm.z_, -1.0f, 1.0f) * q + 0.5f);
// 		return Write(&coords[0], sizeof coords) == sizeof coords;
// 	}

	bool Stream::WriteMatrix3(const Matrix3& value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Stream::WriteMatrix3x4(const Matrix3x4& value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Stream::WriteMatrix4(const Matrix4& value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

// 	bool Stream::WriteColor(const Color& value)
// 	{
// 		return Write(value.Data(), sizeof value) == sizeof value;
// 	}

	bool Stream::WriteBoundingBox(const AABB& value)
	{
		bool success = true;
		success &= WriteVector3(value.mMinimum);
		success &= WriteVector3(value.mMaximum);
		return success;
	}


}


