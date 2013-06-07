#ifndef __BitField_H__
#define __BitField_H__

template<class T>
class BitFieldBase
{
public:
	T flag;

	inline BitFieldBase()
		:flag(0)
	{

	}

	inline void SetBit(T mask,bool val)
	{
		flag &= ~mask;
		flag |= (val ? mask : 0);
	}

	inline bool GetBit(T mask)
	{
		return (flag & mask) != 0;
	}

	inline static void StaticSetBit(T& inFlag, T mask, bool val)
	{
		inFlag &= ~mask;
		inFlag |= (val ? mask : 0);
	}

	inline static bool StaticGetBit(T inFlag,T mask)
	{
		return (inFlag & mask) != 0;
	}

};

typedef BitFieldBase<UINT> BitField;

#endif //__BitField_H__