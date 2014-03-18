#ifndef _GenerateData_H_
#define _GenerateData_H_


namespace ma
{
	inline long GenerateScalar(long min, long max)
	{
		// Note: this is not a very good RNG, but it should be suitable for our purposes.
		long r = 0;
		for (UINT i = 0; i < sizeof(long)/sizeof(int); i++)
		{
			r = r << 8; // sizeof(int) * CHAR_BITS
			r |= rand();
		}

		// Now we have a random long between 0 and MAX_LONG.  We need to clamp it between min and max.
		r %= max - min;
		r += min;

		return r;
	}

	float GenerateScalar(float min, float max)
	{
		return min + (max - min) * MATH_RANDOM_0_1();
	}

	void GenerateVectorInRect(const Vector3& base, const Vector3& variance, Vector3* dst)
	{
		ASSERT(dst);

		// Scale each component of the variance vector by a random float
		// between -1 and 1, then add this to the corresponding base component.
		dst->x = base.x + variance.x * MATH_RANDOM_MINUS1_1();
		dst->y = base.y + variance.y * MATH_RANDOM_MINUS1_1();
		dst->z = base.z + variance.z * MATH_RANDOM_MINUS1_1();
	}

	void GenerateVectorInEllipsoid(const Vector3& center, const Vector3& scale, Vector3* dst)
	{
		ASSERT(dst);

		// Generate a point within a unit cube, then reject if the point is not in a unit sphere.
		do
		{
			dst->x = MATH_RANDOM_MINUS1_1();
			dst->y = MATH_RANDOM_MINUS1_1();
			dst->z = MATH_RANDOM_MINUS1_1();
		} while ( Vec3Length(dst) > 1.0f);

		// Scale this point by the scaling vector.
		dst->x *= scale.x;
		dst->y *= scale.y;
		dst->z *= scale.z;

		// Translate by the center point.
		*dst += center;
	}

	void GenerateVector(const Vector3& base, const Vector3& variance, Vector3* dst, bool ellipsoid)
	{
		if (ellipsoid)
		{
			GenerateVectorInEllipsoid(base, variance, dst);
		}
		else
		{
			GenerateVectorInRect(base, variance, dst);
		}
	}

	void GenerateColor(const Vector4& base, const Vector4& variance, Vector4* dst)
	{
		ASSERT(dst);

		// Scale each component of the variance color by a random float
		// between -1 and 1, then add this to the corresponding base component.
		dst->x = base.x + variance.x * MATH_RANDOM_MINUS1_1();
		dst->y = base.y + variance.y * MATH_RANDOM_MINUS1_1();
		dst->z = base.z + variance.z * MATH_RANDOM_MINUS1_1();
		dst->w = base.w + variance.w * MATH_RANDOM_MINUS1_1();
	}
}

#endif