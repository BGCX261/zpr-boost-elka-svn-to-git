#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Base.hpp"

namespace zpr
{
	/**
	 * Class representing camera object, contains essential information about
	 */
	class Camera
	{
		private:
		/** Camera id */
		const int id_;
		/** Camera position */
		Point position_;
		/** Camera look at direction [radians] */
		double direction_;
		/** Camera view angle [radians] */
		double angle_;
		/** Camera range of seeing */
		double range_;
		/** Camera logging precision */
		double precision_;

		public:
		/**
		 * Camera constructor
		 *
		 * @param id camera id
		 * @param position camera position
		 * @param direction look at direction [degrees]
		 * @param angle camera view angle [degrees]
		 * @param range camera seeing range [m]
		 * @param precision camera logging precision [%]
		 */
		Camera(int id, Point position, double direction, double angle, double range, double precision);

		/**
		 *
		 * @return camera id
		 */
		int id() const;
		
		/**
		 *
		 * @return camera range
		 */
		double range() const;

		/**
		 *
		 * @return camera view angle
		 */
		double angle() const;

		/**
		 *
		 * @return camera id
		 */
		double direction() const;
		
		/**
		 *
		 * @return camera position
		 */
		const Point position() const;
		
		/**
		 * Check if object is in camera seeing field
		 *
		 * @return true if in range, false otherwise
		 */
		bool inRange(const Point & object) const;

		
		/**
		 * Calculate noise from camera precision
		 *
		 * @return logging noise
		 */
		double noise() const;
	};
}

#endif // CAMERA_HPP