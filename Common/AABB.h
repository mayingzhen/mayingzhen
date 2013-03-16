#ifndef  _AABB__H__
#define  _AABB__H__

namespace ma
{
	class COMMON_API AABB
	{
	public:

		D3DXVECTOR3 m_vMin;
		D3DXVECTOR3 m_vMax;

		AABB();

		void SetNull();

		bool IsNull() const;

		void Merge(const D3DXVECTOR3& v);

		void Merge(const D3DXVECTOR3* arrPiont,int n);

		void Merge(const AABB& aabb);

		void Transform(const D3DXMATRIX& mat);

		D3DXVECTOR3 Center() const;

		D3DXVECTOR3 Extent() const;

		D3DXVECTOR3 Size() const;

	};

	inline AABB::AABB()
	{

	}

	inline void AABB::SetNull()
	{
		m_vMin = D3DXVECTOR3(F_MAX,F_MAX,F_MAX);
		m_vMax = -m_vMin;
	}

	inline bool AABB::IsNull() const
	{
		return m_vMin.x >= F_MAX && m_vMax.x <= -F_MAX;
	}

	inline void AABB::Merge(const D3DXVECTOR3& v)
	{
		Vec3Min(&m_vMin,&m_vMin,&v);
		Vec3Max(&m_vMax,&m_vMax,&v);
	}

	inline void AABB::Merge(const D3DXVECTOR3* arrPiont,int n)
	{
		for (int i = 0; i < n; ++i)
		{
			Merge(arrPiont[i]);
		}
	}

	inline void AABB::Merge(const AABB& aabb)
	{
		Vec3Max(&m_vMax,&m_vMax,&aabb.m_vMax);
		Vec3Min(&m_vMin,&m_vMin,&aabb.m_vMin);
	}

	inline D3DXVECTOR3 AABB::Center() const
	{
		D3DXVECTOR3 vCenter;
		vCenter = m_vMin + m_vMax;
		vCenter *= 0.5f;
		return vCenter;
	}

	inline D3DXVECTOR3 AABB::Extent() const
	{
		D3DXVECTOR3 vExtent;
		if (!IsNull())
		{
			vExtent = m_vMax - m_vMin;
			vExtent *= 0.5f;
		}else{
			vExtent = Vec3Zero();
		}
		return vExtent;

	}

	inline D3DXVECTOR3 AABB::Size() const
	{
		D3DXVECTOR3 vExtent;
		if (!IsNull())
		{
			vExtent = m_vMax - m_vMin;
		}else{
			vExtent = Vec3Zero();
		}
		return vExtent;
	}

}

#endif



