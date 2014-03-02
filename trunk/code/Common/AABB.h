#ifndef  _AABB__H__
#define  _AABB__H__

namespace ma
{
	class COMMON_API AABB
	{
	public:

		Vector3 m_vMin;
		Vector3 m_vMax;

		AABB();

		AABB(const Vector3& vMin, const Vector3 vMax);

		void SetNull();

		bool IsNull() const;

		void Merge(const Vector3& v);

		void Merge(const Vector3* arrPiont,int n);

		void Merge(const AABB& aabb);

		void Transform(const Matrix4x4& mat);

		Vector3 Center() const;

		Vector3 Extent() const;

		Vector3 Size() const;

	};

	inline AABB::AABB()
	{
		SetNull();
	}

	inline AABB::AABB(const Vector3& vMin, const Vector3 vMax)
	{
		m_vMin = vMin;
		m_vMax = vMax;
	}

	inline void AABB::SetNull()
	{
		m_vMin = Vector3(FMAX,FMAX,FMAX);
		m_vMax = -m_vMin;
	}

	inline bool AABB::IsNull() const
	{
		return m_vMin.x >= FMAX && m_vMax.x <= -FMAX;
	}

	inline void AABB::Merge(const Vector3& v)
	{
		Vec3Min(&m_vMin,&m_vMin,&v);
		Vec3Max(&m_vMax,&m_vMax,&v);
	}

	inline void AABB::Merge(const Vector3* arrPiont,int n)
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

	inline Vector3 AABB::Center() const
	{
		Vector3 vCenter;
		vCenter = m_vMin + m_vMax;
		vCenter *= 0.5f;
		return vCenter;
	}

	inline Vector3 AABB::Extent() const
	{
		Vector3 vExtent;
		if (!IsNull())
		{
			vExtent = m_vMax - m_vMin;
			vExtent *= 0.5f;
		}else{
			vExtent = Vec3Zero();
		}
		return vExtent;

	}

	inline Vector3 AABB::Size() const
	{
		Vector3 vExtent;
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



