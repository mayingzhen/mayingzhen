#ifndef __Line3D_H__
#define __Line3D_H__

#include "Math.h"
#include "Vector3.h"
namespace ma
{

class Line3D
{
public:
	//! Default constructor
	/** line from (0,0,0) to (1,1,1) */
	Line3D() : start(0,0,0), end(1,1,1) {}
	//! Constructor with two points
	Line3D(float xa, float ya, float za, float xb, float yb, float zb) : start(xa, ya, za), end(xb, yb, zb) {}
	//! Constructor with two points as vectors
	Line3D(const Vector3& start, const Vector3& end) : start(start), end(end) {}

	// operators

	Line3D operator+(const Vector3& point) const { return Line3D(start + point, end + point); }
	Line3D& operator+=(const Vector3& point) { start += point; end += point; return *this; }

	Line3D operator-(const Vector3& point) const { return Line3D(start - point, end - point); }
	Line3D& operator-=(const Vector3& point) { start -= point; end -= point; return *this; }

	bool operator==(const Line3D& other) const
	{ return (start==other.start && end==other.end) || (end==other.start && start==other.end);}
	bool operator!=(const Line3D& other) const
	{ return !(start==other.start && end==other.end) || (end==other.start && start==other.end);}

	// functions
	//! Set this line to a new line going through the two points.
	void setLine(const float& xa, const float& ya, const float& za, const float& xb, const float& yb, const float& zb)
	{start = Vector3(xa, ya, za); end = Vector3(xb, yb, zb);}
	//! Set this line to a new line going through the two points.
	void setLine(const Vector3& nstart, const Vector3& nend)
	{start = nstart; end = nend;}
	//! Set this line to new line given as parameter.
	void setLine(const Line3D& line)
	{start = line.start; end = line.end;}

	//! Get length of line
	/** \return Length of line. */
	float length() const { return start.distance(end); }

	//! Get squared length of line
	/** \return Squared length of line. */
	float squaredLength() const { return start.squaredDistance(end); }

	//! Get middle of line
	/** \return Center of line. */
	Vector3 getMiddle() const
	{
		return (start + end) * (float)0.5;
	}

	//! Get vector of line
	/** \return vector of line. */
	Vector3 getVector() const
	{
		return end - start;
	}

	//! Check if the given point is between start and end of the line.
	/** Assumes that the point is already somewhere on the line.
	\param point The point to test.
	\return True if point is on the line between start and end, else false.
	*/
	bool isPointBetweenStartAndEnd(const Vector3& point) const
	{
		const float f = end.squaredDistance(start);
		return point.squaredDistance(start) <= f &&
			point.squaredDistance(end) <= f;
	}

	//! Get the closest point on this line to a point
	/** \param point The point to compare to.
	\return The nearest point which is part of the line. */
	Vector3 getClosestPoint(const Vector3& point) const
	{
		Vector3 c = point - start;
		Vector3 v = end - start;
		float d = (float)v.length();
		v /= d;
		float t = v.dotProduct(c);

		if (t < (float)0.0)
			return start;
		if (t > d)
			return end;

		v *= t;
		return start + v;
	}

	//! Check if the line intersects with a shpere
	/** \param sorigin: Origin of the shpere.
	\param sradius: Radius of the sphere.
	\param outdistance: The distance to the first intersection point.
	\return True if there is an intersection.
	If there is one, the distance to the first intersection point
	is stored in outdistance. */
	bool getIntersectionWithSphere(Vector3 sorigin, float sradius, float& outdistance) const
	{
		const Vector3 q = sorigin - start;
		float c = q.length();
		float v = q.dotProduct(getVector().normalisedCopy());
		float d = sradius * sradius - (c*c - v*v);

		if (d < 0.0)
			return false;

		outdistance = v - Math::Sqrt( d );
		return true;
	}

	// member variables

	//! Start point of line
	Vector3 start;
	//! End point of line
	Vector3 end;
};

}

#endif
