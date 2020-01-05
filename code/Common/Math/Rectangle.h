#ifndef RECTANGLE_H_
#define RECTANGLE_H_


namespace ma
{

	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup General
	*  @{
	*/

    struct Rectangle
    {
        float left;
		float top;
		float right;
		float bottom;

		Rectangle()
		{
			left = 0.0f;
			top = 0.0f;
			right = 0.0f;
			bottom = 0.0f;
		}

		Rectangle(float fLeft,float fTop,float fRight,float fBottom)
		{
			left = fLeft;
			top = fTop;
			right = fRight;
			bottom = fBottom;
		}

		bool IsEmpty() const {return left == 0.0f && top == 0.0f && right == 0.0f && bottom == 0.0f;}

		float offsetX() const { return left; }
		float offsetY() const { return top; }
		float width() const {return right - left;}
		float height() const {return bottom - top;}

        inline bool inside(float x, float y) const { return x >= left && x <= right && y >= top && y <= bottom; }
    };

    /** Geometric intersection of two rectanglar regions.
     *
     * @remarks Calculates the geometric intersection of two rectangular
     * regions.  Rectangle coordinates must be ([0-N], [0-N]), such that
     * (0,0) is in the upper left hand corner.
     *
     * If the two input rectangles do not intersect, then the result will be
     * a degenerate rectangle, i.e. left >= right or top >= bottom, or both.
     */
    inline Rectangle intersect(const Rectangle& lhs, const Rectangle& rhs)
    {
        Rectangle r;

        r.left   = lhs.left   > rhs.left   ? lhs.left   : rhs.left;
        r.top    = lhs.top    > rhs.top    ? lhs.top    : rhs.top;
        r.right  = lhs.right  < rhs.right  ? lhs.right  : rhs.right;
        r.bottom = lhs.bottom < rhs.bottom ? lhs.bottom : rhs.bottom;

        return r;
    }
	/** @} */
	/** @} */

}

#endif
