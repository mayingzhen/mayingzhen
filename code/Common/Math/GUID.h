#ifndef __maGUID_H__
#define __maGUID_H__

namespace ma
{
	struct maGUID
	{
		uint64_t m_a;
		uint64_t m_b;

		maGUID()
		{
			m_a = 0;
			m_b = 0;
		}

		bool operator ==(const maGUID& rhs) const
		{
			return (m_a == rhs.m_a) && (m_b == rhs.m_b);
		}

		bool operator !=(const maGUID& rhs) const
		{
			return !(*this == rhs);
		}

		bool operator <(const maGUID& rhs) const
		{
			return (m_a < rhs.m_a ? true : (m_a == rhs.m_a ? m_b < rhs.m_b : false) );
		}
	};
}

#endif
