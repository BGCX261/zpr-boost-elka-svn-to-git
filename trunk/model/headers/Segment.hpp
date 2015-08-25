#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include "Base.hpp"

namespace zpr
{
	/**
	  * Abstract base class for representing track's segments.
	  */
	class Segment
	{
		protected:
		/** Segment begin */
		Point begin_;
		/** Segment end */
		Point end_;

		public:
		/**
		 * Segment constructor
		 *
		 * @param begin begin point
		 * @param end end point
		 */
		Segment(const Point begin, const Point end);
		virtual ~Segment() = 0;
		
		/**
		 * Calculate position on segment by given % of it
		 *
		 * @param percent percent of segment (0..1)
		 * @return calculated position
		 */
		virtual Point position(double percent) = 0;

		/**
		 *
		 * @return length of segment
		 */
		virtual double length() = 0;
		
		/**
		 * Check if its bezier segment or not
		 *
		 * @return segment angle between begin, center, end
		 */
		virtual double bezier() = 0;

		/**
		 *
		 * @return begin of segment
		 */
		Point begin();
		
		/**
		 *
		 * @return end of segment
		 */
		Point end();
	};
	
	/**
	  * Class for representing arc (bezier) segments.
	  */
	class BezierSegment : public Segment
	{
		/** Bezier control point */
		Point control_;
		public:
		/**
		 * Bezier segment constructor
		 *
		 * @param begin begin point
		 * @param end end point
		 * @param control control point
		 */
		BezierSegment(const Point begin, const Point end, const Point control);
		
		/**
		 * Calculate position on segment by given % of it
		 *
		 * @param percent percent of segment (0..1)
		 * @return calculated position
		 */
		Point position(double percent);

		/**
		 *
		 * @return length of segment
		 */
		double length();
		
		/**
		 * Calculate bezier curve angle
		 *
		 * @return segment angle between begin, control point, end
		 */
		double bezier();

		/**
		 *
		 * @return control point
		 */
		Point control();
	};
	
	/**
	  * Class for representing straight segments.
	  */
	class StraightSegment : public Segment
	{
		public:
		/**
		 * Straight segment constructor
		 *
		 * @param begin begin point
		 * @param end end point
		 */
		StraightSegment(const Point begin, const Point end);
		
		/**
		 *
		 * @return length of segment
		 */
		double length();

		/**
		 *
		 * @return 0
		 */
		double bezier();
		
		/**
		 * Calculate position on segment by given % of it
		 *
		 * @param percent percent of segment (0..1)
		 * @return calculated position
		 */
		Point position(double percent);
	};
}

#endif // SEGMENT_HPP