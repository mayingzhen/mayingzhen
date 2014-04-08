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
		size_t chunkSize = Min(maxCount, (UINT)128-1);
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
			chunkSize = Min(maxCount-totalCount, (size_t)128-1);
		}

		// Terminate
		if(buf)
			buf[totalCount] = '\0';

		return totalCount;
	}
}


