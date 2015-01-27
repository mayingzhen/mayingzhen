#pragma once
#include "AABB.h"
#include "Line3D.h"
#include "Math.h"
#include "Plane.h"
namespace ma
{

class Triangle3D
{
public:
	Triangle3D(void){}
	//! Constructor for triangle with given three vertices
	Triangle3D(Vector3 v1, Vector3 v2, Vector3 v3) : pointA(v1), pointB(v2), pointC(v3) {}

	//! Equality operator
	bool operator==(const Triangle3D& other) const
	{
		return other.pointA==pointA && other.pointB==pointB && other.pointC==pointC;
	}

	//! Inequality operator
	bool operator!=(const Triangle3D& other) const
	{
		return !(*this==other);
	}

	//! Determines if the triangle is totally inside a bounding box.
	/** \param box Box to check.
	\return True if triangle is within the box, otherwise false. */
	bool isTotalInsideBox(const AABB& box) const
	{
		return box.contains(pointA) && box.contains(pointB) && box.contains(pointC);
	}

	//! Determines if the triangle is totally outside a bounding box.
	/** \param box Box to check.
	\return True if triangle is outside the box, otherwise false. */
	bool isTotalOutsideBox(const AABB& box) const
	{
		return ((pointA.x > box.getMaximum().x && pointB.x > box.getMaximum().x && pointC.x > box.getMaximum().x) ||

			(pointA.y > box.getMaximum().y && pointB.y > box.getMaximum().y && pointC.y > box.getMaximum().y) ||
			(pointA.z > box.getMaximum().z && pointB.z > box.getMaximum().z && pointC.z > box.getMaximum().z) ||
			(pointA.x < box.getMinimum().x && pointB.x < box.getMinimum().x && pointC.x < box.getMinimum().x) ||
			(pointA.y < box.getMinimum().y && pointB.y < box.getMinimum().y && pointC.y < box.getMinimum().y) ||
			(pointA.z < box.getMinimum().z && pointB.z < box.getMinimum().z && pointC.z < box.getMinimum().z));
	}

	//! Get the closest point on a triangle to a point on the same plane.
	/** \param p Point which must be on the same plane as the triangle.
	\return The closest point of the triangle */
	Vector3 closestPointOnTriangle(const Vector3& p) const
	{
		const Vector3 rab = Line3D(pointA, pointB).getClosestPoint(p);
		const Vector3 rbc = Line3D(pointB, pointC).getClosestPoint(p);
		const Vector3 rca = Line3D(pointC, pointA).getClosestPoint(p);

		const float d1 = rab.distance(p);
		const float d2 = rbc.distance(p);
		const float d3 = rca.distance(p);

		if (d1 < d2)
			return d1 < d3 ? rab : rca;

		return d2 < d3 ? rbc : rca;
	}

	//! Check if a point is inside the triangle
	/** \param p Point to test. Assumes that this point is already
	on the plane of the triangle.
	\return True if the point is inside the triangle, otherwise false. */
	bool isPointInside(const Vector3& p) const
	{
		return (isOnSameSide(p, pointA, pointB, pointC) &&
			isOnSameSide(p, pointB, pointA, pointC) &&
			isOnSameSide(p, pointC, pointA, pointB));
	}

	//! Check if a point is inside the triangle.
	/** This method is an implementation of the example used in a
	paper by Kasper Fauerby original written by Keidy from
	Mr-Gamemaker.
	\param p Point to test. Assumes that this point is already
	on the plane of the triangle.
	\return True if point is inside the triangle, otherwise false. */
	bool isPointInsideFast(const Vector3& p) const
	{
		const Vector3 f = pointB - pointA;
		const Vector3 g = pointC - pointA;

		const float a = f.dotProduct(f);
		const float b = f.dotProduct(g);
		const float c = g.dotProduct(g);

		const Vector3 vp = p - pointA;
		const float d = vp.dotProduct(f);
		const float e = vp.dotProduct(g);

		float x = (d*c)-(e*b);
		float y = (e*a)-(d*b);
		const float ac_bb = (a*c)-(b*b);
		float z = x+y-ac_bb;

		// return sign(z) && !(sign(x)||sign(y))
		return (( (uint32(z)) & ~((uint32(x))|(uint32(y))) ) & 0x80000000)!=0;
	}

	//! Get an intersection with a 3d line.
	/** \param line Line to intersect with.
	\param outIntersection Place to store the intersection point, if there is one.
	\return True if there was an intersection, false if not. */
	bool getIntersectionWithLimitedLine(const Line3D& line,
		Vector3& outIntersection) const
	{
		if(!getIntersectionWithLine(line.start,
			line.getVector(), outIntersection))
		{
			return false;
		}

		const float f = line.end.squaredDistance(line.start);
		return outIntersection.squaredDistance(line.start) <= f &&
			outIntersection.squaredDistance(line.end) <= f;
	}


	//! Get an intersection with a 3d line.
	/** Please note that also points are returned as intersection which
	are on the line, but not between the start and end point of the line.
	If you want the returned point be between start and end
	use getIntersectionWithLimitedLine().
	\param linePoint Point of the line to intersect with.
	\param lineVect Vector of the line to intersect with.
	\param outIntersection Place to store the intersection point, if there is one.
	\return True if there was an intersection, false if there was not. */
	bool getIntersectionWithLine(const Vector3& linePoint,
		const Vector3& lineVect, Vector3& outIntersection) const
	{
		if (getIntersectionOfPlaneWithLine(linePoint, lineVect, outIntersection))
			return isPointInside(outIntersection);

		return false;
	}


	//! Calculates the intersection between a 3d line and the plane the triangle is on.
	/** \param lineVect Vector of the line to intersect with.
	\param linePoint Point of the line to intersect with.
	\param outIntersection Place to store the intersection point, if there is one.
	\return True if there was an intersection, else false. */
	bool getIntersectionOfPlaneWithLine(const Vector3& linePoint,
		const Vector3& lineVect, Vector3& outIntersection) const
	{
		const Vector3 normal = getNormal().normalisedCopy();
		float t2;

		if ( Math::RealEqual(( t2 = normal.dotProduct(lineVect) ), 0.f))
			return false;

		float d = pointA.dotProduct(normal);
		float t = -(normal.dotProduct(linePoint) - d) / t2;
		outIntersection = linePoint + (lineVect * t);
		return true;
	}


	//! Get the normal of the triangle.
	/** Please note: The normal is not always normalized. */
	Vector3 getNormal() const
	{
		return (pointB - pointA).crossProduct(pointC - pointA);
	}

	//! Test if the triangle would be front or backfacing from any point.
	/** Thus, this method assumes a camera position from which the
	triangle is definitely visible when looking at the given direction.
	Do not use this method with points as it will give wrong results!
	\param lookDirection Look direction.
	\return True if the plane is front facing and false if it is backfacing. */
	bool isFrontFacing(const Vector3& lookDirection) const
	{
		const Vector3 n = getNormal().normalisedCopy();
		const float d = (float)n.dotProduct(lookDirection);
		return d <= 0.f;
	}

	//! Get the plane of this triangle.
	Plane getPlane() const
	{
		return Plane(pointA, pointB, pointC);
	}

	//! Get the area of the triangle
	float getArea() const
	{
		return (pointB - pointA).crossProduct(pointC - pointA).length() * 0.5f;

	}

	//! sets the triangle's points
	void set(const Vector3& a, const Vector3& b, const Vector3& c)
	{
		pointA = a;
		pointB = b;
		pointC = c;
	}
private:
	bool isOnSameSide(const Vector3& p1, const Vector3& p2,
		const Vector3& a, const Vector3& b) const
	{
		Vector3 bminusa = b - a;
		Vector3 cp1 = bminusa.crossProduct(p1 - a);
		Vector3 cp2 = bminusa.crossProduct(p2 - a);
		return (cp1.dotProduct(cp2) >= 0.0f);
	}

public:
	Vector3 pointA;
	Vector3 pointB;
	Vector3 pointC;
};

}