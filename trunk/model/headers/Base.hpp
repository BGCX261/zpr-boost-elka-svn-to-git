#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>

namespace zpr
{
	/** Objects occuring in simulator */
	enum OBJECTS { WALKER, SMALLCAR, BIGCAR, CAMERA };

	/**
	  * Basic structure identyfing object's position
	  */
	struct Point
	{
		/** X coordinate */
		double x_;
		/** Y coordinate */
		double y_;

		/** Point c-tor: default (0, 0) */
		Point(double x = 0.0, double y = 0.0);

		/** Checks equality of coordinates */
		bool operator==(const Point & other);

		/** Adds each coordinates into point object */
		Point & operator+=(const Point & add);

		/**
		 * Calculate distance between given points
		 *
		 * @param p1 one point
		 * @param p2 other point
		 * @return distance
		 */
		static double distance(const Point &p1, const Point & p2);

		/**
		 * Calculate angle change (vector angle)
		 * Order important
		 *
		 * @param p1 first point
		 * @param p2 second point
		 * @return angle
		 */
		static double angle(const Point &p1, const Point & p2);

		/**
		 * Convert point information into string
		 *
		 * @return formed information about point (x, y) [.3]
		 */
		std::string str() const;
	};

	/**
	 * Adds point coordinates
	 *
	 * @param p1 one point
	 * @param p2 other point
	 * @return Point summary
	 */
	Point operator+(Point p1, const Point & p2);

	/**
	 * Checks if given object is 'located' between given boundaries
	 *
	 * @param downBound first boundary
	 * @param upBound second boundary
	 * @param value type to check
	 * @return true if in range, false otherwise
	 */
	template<typename T>
	bool inRange(const T & downBound, const T & upBound, const T & value)
	{
		return downBound <= value && value <= upBound;
	}
}

#endif // BASE_HPP