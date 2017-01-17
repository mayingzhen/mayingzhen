#pragma once


#include "Vector2.h"

namespace ma 
{
	class AABB2D
	{
	public:
		enum Extent
		{
			EXTENT_NULL,
			EXTENT_FINITE,
			EXTENT_INFINITE
		};
	public:

		Vector2 mMinimum;
		Vector2 mMaximum;
		Extent mExtent;
		mutable Vector2* mpCorners;

	public:

		typedef enum {
			LEFT_BOTTOM = 0,
			LEFT_TOP = 1,
			RIGHT_TOP = 2,
			RIGHT_BOTTOM = 3,
		} CornerEnum;
		inline AABB2D() : mMinimum(Vector2::ZERO), mMaximum(Vector2::UNIT_SCALE), mpCorners(0)
		{
			// Default to a null box 
			setMinimum( -0.5, -0.5);
			setMaximum( 0.5, 0.5);
			mExtent = EXTENT_NULL;
		}
		inline AABB2D(Extent e) : mMinimum(Vector2::ZERO), mMaximum(Vector2::UNIT_SCALE), mpCorners(0)
		{
			setMinimum( -0.5, -0.5);
			setMaximum( 0.5, 0.5);
			mExtent = e;
		}

		inline AABB2D(const AABB2D & rkBox) : mMinimum(Vector2::ZERO), mMaximum(Vector2::UNIT_SCALE), mpCorners(0)

		{
			if (rkBox.isNull())
				setNull();
			else if (rkBox.isInfinite())
				setInfinite();
			else
				setExtents( rkBox.mMinimum, rkBox.mMaximum );
		}

		inline AABB2D( const Vector2& min, const Vector2& max ) : mMinimum(Vector2::ZERO), mMaximum(Vector2::UNIT_SCALE), mpCorners(0)
		{
			setExtents( min, max );
		}

		inline AABB2D(
			float mx, float my,
			float Mx, float My ) : mMinimum(Vector2::ZERO), mMaximum(Vector2::UNIT_SCALE), mpCorners(0)
		{
			setExtents( mx, my, Mx, My );
		}

		AABB2D& operator=(const AABB2D& rhs)
		{
			// Specifically override to avoid copying mpCorners
			if (rhs.isNull())
				setNull();
			else if (rhs.isInfinite())
				setInfinite();
			else
				setExtents(rhs.mMinimum, rhs.mMaximum);

			return *this;
		}

		~AABB2D()
		{
			if (mpCorners)
				SAFE_DELETE_ARRAY(mpCorners);//OGRE_FREE(mpCorners, MEMCATEGORY_SCENE_CONTROL);
		}


		/** Gets the minimum corner of the box.
		*/
		inline const Vector2& getMinimum(void) const
		{ 
			return mMinimum; 
		}

		/** Gets a modifiable version of the minimum
		corner of the box.
		*/
		inline Vector2& getMinimum(void)
		{ 
			return mMinimum; 
		}

		/** Gets the maximum corner of the box.
		*/
		inline const Vector2& getMaximum(void) const
		{ 
			return mMaximum;
		}

		/** Gets a modifiable version of the maximum
		corner of the box.
		*/
		inline Vector2& getMaximum(void)
		{ 
			return mMaximum;
		}


		/** Sets the minimum corner of the box.
		*/
		inline void setMinimum( const Vector2& vec )
		{
			mExtent = EXTENT_FINITE;
			mMinimum = vec;
		}

		inline void setMinimum( float x, float y )
		{
			mExtent = EXTENT_FINITE;
			mMinimum.x = x;
			mMinimum.y = y;
		}

		/** Changes one of the components of the minimum corner of the box
		used to resize only one dimension of the box
		*/
		inline void setMinimumX(float x)
		{
			mMinimum.x = x;
		}

		inline void setMinimumY(float y)
		{
			mMinimum.y = y;
		}

		/** Sets the maximum corner of the box.
		*/
		inline void setMaximum( const Vector2& vec )
		{
			mExtent = EXTENT_FINITE;
			mMaximum = vec;
		}

		inline void setMaximum( float x, float y )
		{
			mExtent = EXTENT_FINITE;
			mMaximum.x = x;
			mMaximum.y = y;
		}

		/** Changes one of the components of the maximum corner of the box
		used to resize only one dimension of the box
		*/
		inline void setMaximumX( float x )
		{
			mMaximum.x = x;
		}

		inline void setMaximumY( float y )
		{
			mMaximum.y = y;
		}

		/** Sets both minimum and maximum extents at once.
		*/
		inline void setExtents( const Vector2& min, const Vector2& max )
		{
            ASSERT( (min.x <= max.x && min.y <= max.y) &&
                "The minimum corner of the box must be less than or equal to maximum corner" );

			mExtent = EXTENT_FINITE;
			mMinimum = min;
			mMaximum = max;
		}

		inline void setExtents(
			float mx, float my,
			float Mx, float My)
		{
            ASSERT( (mx <= Mx && my <= My) &&
                "The minimum corner of the box must be less than or equal to maximum corner" );

			mExtent = EXTENT_FINITE;

			mMinimum.x = mx;
			mMinimum.y = my;

			mMaximum.x = Mx;
			mMaximum.y = My;

		}

		inline const Vector2* getAllCorners(void) const
		{
			ASSERT( (mExtent == EXTENT_FINITE) && "Can't get corners of a null or infinite AAB" );

			// The order of these items is, using right-handed co-ordinates:
			// Minimum Z face, starting with Min(all), then anticlockwise
			//   around face (looking onto the face)
			// Maximum Z face, starting with Max(all), then anticlockwise
			//   around face (looking onto the face)
			// Only for optimization/compatibility.
			if (!mpCorners)
				mpCorners = new Vector2[4];//(Vector2, 8, MEMCATEGORY_SCENE_CONTROL);

			mpCorners[0] = mMinimum;
			mpCorners[1].x = mMinimum.x; mpCorners[1].y = mMaximum.y;
			mpCorners[2].x = mMaximum.x; mpCorners[2].y = mMaximum.y;
			mpCorners[3].x = mMaximum.x; mpCorners[3].y = mMinimum.y;

			mpCorners[4] = mMaximum;
			mpCorners[5].x = mMinimum.x; mpCorners[5].y = mMaximum.y;
			mpCorners[6].x = mMinimum.x; mpCorners[6].y = mMinimum.y;
			mpCorners[7].x = mMaximum.x; mpCorners[7].y = mMinimum.y;

			return mpCorners;
		}

		/** gets the position of one of the corners
		*/
		Vector2 getCorner(CornerEnum cornerToGet) const
		{
			switch(cornerToGet)
			{
			case LEFT_BOTTOM:
				return mMinimum;
			case LEFT_TOP:
				return Vector2(mMinimum.x, mMaximum.y);
			case RIGHT_TOP:
				return Vector2(mMaximum.x, mMaximum.y);
			case RIGHT_BOTTOM:
				return Vector2(mMaximum.x, mMinimum.y);
			default:
				return Vector2();
			}
		}

		friend std::ostream& operator<<( std::ostream& o, const AABB2D aab )
		{
			switch (aab.mExtent)
			{
			case EXTENT_NULL:
				o << "AABB(null)";
				return o;

			case EXTENT_FINITE:
				o << "AABB(min=" << aab.mMinimum << ", max=" << aab.mMaximum << ")";
				return o;

			case EXTENT_INFINITE:
				o << "AABB(infinite)";
				return o;

			default: // shut up compiler
				assert( false && "Never reached" );
				return o;
			}
		}

		/** Merges the passed in box into the current box. The result is the
		box which encompasses both.
		*/
		void merge( const AABB2D& rhs )
		{
			// Do nothing if rhs null, or this is infinite
			if ((rhs.mExtent == EXTENT_NULL) || (mExtent == EXTENT_INFINITE))
			{
				return;
			}
			// Otherwise if rhs is infinite, make this infinite, too
			else if (rhs.mExtent == EXTENT_INFINITE)
			{
				mExtent = EXTENT_INFINITE;
			}
			// Otherwise if current null, just take rhs
			else if (mExtent == EXTENT_NULL)
			{
				setExtents(rhs.mMinimum, rhs.mMaximum);
			}
			// Otherwise merge
			else
			{
				Vector2 min = mMinimum;
				Vector2 max = mMaximum;
				max.makeCeil(rhs.mMaximum);
				min.makeFloor(rhs.mMinimum);

				setExtents(min, max);
			}

		}

		/** Extends the box to encompass the specified point (if needed).
		*/
		inline void merge( const Vector2& point )
		{
			switch (mExtent)
			{
			case EXTENT_NULL: // if null, use this point
				setExtents(point, point);
				return;

			case EXTENT_FINITE:
				mMaximum.makeCeil(point);
				mMinimum.makeFloor(point);
				return;

			case EXTENT_INFINITE: // if infinite, makes no difference
				return;
			}

			ASSERT( false && "Never reached" );
		}

		/** Sets the box to a 'null' value i.e. not a box.
		*/
		inline void setNull()
		{
			mExtent = EXTENT_NULL;
		}

		/** Returns true if the box is null i.e. empty.
		*/
		inline bool isNull(void) const
		{
			return (mExtent == EXTENT_NULL);
		}

		/** Returns true if the box is finite.
		*/
		bool isFinite(void) const
		{
			return (mExtent == EXTENT_FINITE);
		}

		/** Sets the box to 'infinite'
		*/
		inline void setInfinite()
		{
			mExtent = EXTENT_INFINITE;
		}

		/** Returns true if the box is infinite.
		*/
		bool isInfinite(void) const
		{
			return (mExtent == EXTENT_INFINITE);
		}

		/** Returns whether or not this box intersects another. */
		inline bool intersects(const AABB2D& b2) const
		{
			// Early-fail for nulls
			if (this->isNull() || b2.isNull())
				return false;

			// Early-success for infinites
			if (this->isInfinite() || b2.isInfinite())
				return true;

			// Use up to 6 separating planes
			if (mMaximum.x < b2.mMinimum.x)
				return false;

			if (mMaximum.y < b2.mMinimum.y)
				return false;

			if (mMinimum.x > b2.mMaximum.x)
				return false;

			if (mMinimum.y > b2.mMaximum.y)
				return false;

			// otherwise, must be intersecting
			return true;

		}

		/// Calculate the area of intersection of this box and another
		inline AABB2D intersection(const AABB2D& b2) const
		{
            if (this->isNull() || b2.isNull())
			{
				return AABB2D();
			}
			else if (this->isInfinite())
			{
				return b2;
			}
			else if (b2.isInfinite())
			{
				return *this;
			}

			Vector2 intMin = mMinimum;
            Vector2 intMax = mMaximum;

            intMin.makeCeil(b2.getMinimum());
            intMax.makeFloor(b2.getMaximum());

            // Check intersection isn't null
            if (intMin.x < intMax.x &&
                intMin.y < intMax.y)
            {
                return AABB2D(intMin, intMax);
            }

            return AABB2D();
		}

		/// Calculate the volume of this box
		float volume(void) const
		{
			switch (mExtent)
			{
			case EXTENT_NULL:
				return 0.0f;

			case EXTENT_FINITE:
				{
					Vector2 diff = mMaximum - mMinimum;
					return diff.x * diff.y;
				}

			case EXTENT_INFINITE:
				return Math::POS_INFINITY;

			default: // shut up compiler
				ASSERT( false && "Never reached" );
				return 0.0f;
			}
		}

		/** Scales the AABB by the vector given. */
		inline void scale(const Vector2& s)
		{
			// Do nothing if current null or infinite
			if (mExtent != EXTENT_FINITE)
				return;

			// NB assumes centered on origin
			Vector2 min = mMinimum * s;
			Vector2 max = mMaximum * s;
			setExtents(min, max);
		}

		/** Tests whether the vector point is within this box. */
		bool intersects(const Vector2& v) const
		{
			switch (mExtent)
			{
			case EXTENT_NULL:
				return false;

			case EXTENT_FINITE:
				return(v.x >= mMinimum.x  &&  v.x <= mMaximum.x  && 
					v.y >= mMinimum.y  &&  v.y <= mMaximum.y);

			case EXTENT_INFINITE:
				return true;

			default: // shut up compiler
				ASSERT( false && "Never reached" );
				return false;
			}
		}
		/// Gets the centre of the box
		Vector2 getCenter(void) const
		{
			ASSERT( (mExtent == EXTENT_FINITE) && "Can't get center of a null or infinite AAB" );

			return Vector2(
				(mMaximum.x + mMinimum.x) * 0.5f,
				(mMaximum.y + mMinimum.y) * 0.5f);
		}
		/// Gets the size of the box
		Vector2 getSize(void) const
		{
			switch (mExtent)
			{
			case EXTENT_NULL:
				return Vector2::ZERO;

			case EXTENT_FINITE:
				return mMaximum - mMinimum;

			case EXTENT_INFINITE:
				return Vector2(
					Math::POS_INFINITY,
					Math::POS_INFINITY);

			default: // shut up compiler
				assert( false && "Never reached" );
				return Vector2::ZERO;
			}
		}
		/// Gets the half-size of the box
		Vector2 getHalfSize(void) const
		{
			switch (mExtent)
			{
			case EXTENT_NULL:
				return Vector2::ZERO;

			case EXTENT_FINITE:
				return (mMaximum - mMinimum) * 0.5;

			case EXTENT_INFINITE:
				return Vector2(
					Math::POS_INFINITY,
					Math::POS_INFINITY);

			default: // shut up compiler
				assert( false && "Never reached" );
				return Vector2::ZERO;
			}
		}

        /** Tests whether the given point contained by this box.
        */
        bool contains(const Vector2& v) const
        {
            if (isNull())
                return false;
            if (isInfinite())
                return true;

            return mMinimum.x <= v.x && v.x <= mMaximum.x &&
                   mMinimum.y <= v.y && v.y <= mMaximum.y;
        }

        /** Tests whether another box contained by this box.
        */
        bool contains(const AABB2D& other) const
        {
            if (other.isNull() || this->isInfinite())
                return true;

            if (this->isNull() || other.isInfinite())
                return false;

            return this->mMinimum.x <= other.mMinimum.x &&
                   this->mMinimum.y <= other.mMinimum.y &&
                   other.mMaximum.x <= this->mMaximum.x &&
                   other.mMaximum.y <= this->mMaximum.y ;
        }

        /** Tests 2 boxes for equality.
        */
        bool operator== (const AABB2D& rhs) const
        {
            if (this->mExtent != rhs.mExtent)
                return false;

            if (!this->isFinite())
                return true;

            return this->mMinimum == rhs.mMinimum &&
                   this->mMaximum == rhs.mMaximum;
        }

        /** Tests 2 boxes for inequality.
        */
        bool operator!= (const AABB2D& rhs) const
        {
            return !(*this == rhs);
        }

		// special values
		static const AABB2D BOX_NULL;
		static const AABB2D BOX_INFINITE;


	};
} 

