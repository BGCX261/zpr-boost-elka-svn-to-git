#ifndef TRACK_HPP
#define TRACK_HPP

#include "Segment.hpp"
#include <deque>
#include <boost/shared_ptr.hpp>

namespace zpr
{
	/**
	 * Class representing object route
	 */
	class Track
	{
		public:
		typedef boost::shared_ptr<Segment> PSegment;
		typedef std::deque<PSegment> DSegments;

		protected:
		/** Track segments */
		DSegments segments_;
		/** Length of entire track */
		double length_;

		public:
		/**
		 * Track c-tor
		 */
		Track();
		virtual ~Track() = 0;
		
		/**
		 * Adds point into track
		 *
		 * @param point next track point
		 */
		virtual void addPoint(Point point) = 0;
		
		/**
		 * Calculates position on track from given track percent
		 *
		 * @param percent percent of track
		 * @return Request position, on bezier segment or not
		 */
		std::pair<Point, double> positionOnTrack(double percent);

		/**
		 *
		 * @param percent percent of track
		 * @return percent of track where is the nearest start of bezier position
		 *			and that segmnet angle
		 */
		std::pair<double, double> nextBezierDistanceAngle(double percent);
		
		/**
		 * @return Begin track point
		 */
		Point start();
		
		/**
		 * @return End track point
		 */
		Point finish();
		
		/**
		 * @return Actual track length
		 */
		double length();

		protected:
		/**
		 * Recalculates length of track
		 */
		void recalculateLength();
	};

	/**
	 * Class representing vehicles route
	 * Curve = arc (bezier)
	 */
	class VehicleTrack : public Track
	{
		public:
		/** Corner division amount - curve smoothness */
		static const double BEZIER_DIVISION;

		/**
		 * Adds point into track and calculate arcs on the route
		 *
		 * @param point next point
		 */
		void addPoint(Point point);

		/**
		 * Calculates how far separate corners to make closest to BEZIER_DIVISON division
		 * If it's unable divide by half
		 *
		 * @param start begin point
		 * @param final end point
		 * @param control bezier control point
		 */
		static std::pair<Point, Point> bothBezierBetween(const Point &start, const Point &final, const Point &control);
	};
	
	/**
	 * Class representing walkers route
	 * Curve => acute angles
	 */
	class WalkerTrack : public Track
	{
		public:
		/**
		 * Adds point into track
		 *
		 * @param point next point
		 */
		void addPoint(Point point);
	};
}

#endif // TRACK_HPP