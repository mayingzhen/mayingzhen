/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __Sphere_H_
#define __Sphere_H_

// Precompiler options
//#include "Prerequisites.h"

#include "Vector3.h"

namespace ma {


	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/
	/** A sphere primitive, mostly used for bounds checking. 
    @remarks
        A sphere in math texts is normally represented by the function
        x^2 + y^2 + z^2 = r^2 (for sphere's centered on the origin). Ogre stores spheres
        simply as a center point and a radius.
    */
    class COMMON_API Sphere
    {
    protected:
        float mRadius;
        Vector3 mCenter;
    public:
        /** Standard constructor - creates a unit sphere around the origin.*/
        Sphere() : mRadius(1.0), mCenter(Vector3::ZERO) {}
        /** Constructor allowing arbitrary spheres. 
            @param center The center point of the sphere.
            @param radius The radius of the sphere.
        */
        Sphere(const Vector3& center, float radius)
            : mRadius(radius), mCenter(center) {}

        /** Returns the radius of the sphere. */
        float getRadius(void) const { return mRadius; }

        /** Sets the radius of the sphere. */
        void setRadius(float radius) { mRadius = radius; }

        /** Returns the center point of the sphere. */
        const Vector3& getCenter(void) const { return mCenter; }

        /** Sets the center point of the sphere. */
        void setCenter(const Vector3& center) { mCenter = center; }

		/** Returns whether or not this sphere intersects another sphere. */
		bool intersects(const Sphere& s) const
		{
            return (s.mCenter - mCenter).squaredLength() <=
                Math::Sqr(s.mRadius + mRadius);
		}
		/** Returns whether or not this sphere intersects a box. */
		bool intersects(const AABB& box) const
		{
			return Math::intersects(*this, box);
		}
		/** Returns whether or not this sphere intersects a plane. */
		bool intersects(const Plane& plane) const
		{
			return Math::intersects(*this, plane);
		}
		/** Returns whether or not this sphere intersects a point. */
		bool intersects(const Vector3& v) const
		{
            return ((v - mCenter).squaredLength() <= Math::Sqr(mRadius));
		}
        
		void merge(const Sphere& sphere)
		{
			// Calculate the distance between the two centers.
			float vx = mCenter.x - sphere.mCenter.x;
			float vy = mCenter.y - sphere.mCenter.y;
			float vz = mCenter.z - sphere.mCenter.z;
			float d = sqrt(vx * vx + vy * vy + vz * vz);

			// If one sphere is contained inside the other, set to the larger sphere.
			if (d <= (sphere.mRadius - mRadius))
			{
				mCenter = sphere.mCenter;
				mRadius = sphere.mRadius;
				return;
			}
			else if (d <= (mRadius - sphere.mRadius))
			{
				return;
			}

			// Calculate the unit vector between the two centers.
			assert(d != 0.0f);
			float dI = 1.0f / d;
			vx *= dI;
			vy *= dI;
			vz *= dI;

			// Calculate the new radius.
			float r = (mRadius + sphere.mRadius + d) * 0.5f;

			// Calculate the new mCenter.
			float scaleFactor = (r - sphere.mRadius);
			vx = vx * scaleFactor + sphere.mCenter.x;
			vy = vy * scaleFactor + sphere.mCenter.y;
			vz = vz * scaleFactor + sphere.mCenter.z;

			// Set the new mCenter and radius.
			mCenter.x = vx;
			mCenter.y = vy;
			mCenter.z = vz;
			mRadius = r;
		}
    };
	/** @} */
	/** @} */

}

#endif

