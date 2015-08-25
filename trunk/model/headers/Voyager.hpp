#ifndef VOYAGER_HPP
#define VOYAGER_HPP

#include "Base.hpp"
#include "Track.hpp"
#include <boost/shared_ptr.hpp>

namespace zpr
{
	/**
	 * Abstract base class for all objects moving around city.
	 */
	class Voyager
	{
		public:
		typedef boost::shared_ptr<Track> PTrack;

		protected:
		/** Object position [m] */
		Point position_;
		/** Object velocity [m/s] */
		double velocity_;
		/** Object direction [radians] */
		double angle_;
		/** Percent of traveled track distance [%] */
		double percentDistanceTraveled_;
		/** Object route */
		PTrack track_;
		/** Finished track flag */
		bool finished_;

		public:
		/**
		 * Voyager constructor.
		 *
		 * @param track object track
		 */
		Voyager(const PTrack & track);
		virtual ~Voyager() = 0;
		
		/**
		 * Check object type (enum)
		 *
		 * @return this object type
		 */
		virtual OBJECTS type() const = 0;
		
		/**
		 *
		 * @return object id
		 */
		virtual const std::string& id() const = 0;

		/**
		 *
		 * @return actual position
		 */
		const Point& position() const;

		/**
		 *
		 * @return actual direction
		 */
		double angle() const;
		
		/**
		 *
		 * @return actual velocity
		 */
		double velocity() const;

		/**
		 * Reset object state
		 */
		virtual void reset() = 0;

		/**
		 * Next simulator iteration
		 * Object moves depend on elapsed time
		 *
		 * @param elapsed_time time which elapsed since last update (or to simulate movement)
		 */
		virtual void move(long long int elapsed_time) = 0;

		/**
		 *
		 * @return true if object finished track, false otherwise
		 */
		bool finished();
	};
}

#endif // VOYAGER_HPP