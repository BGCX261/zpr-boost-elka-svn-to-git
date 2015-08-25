#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "Camera.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <deque>

namespace zpr
{	
	typedef boost::tuple<Point, double, OBJECTS, std::string, double> TObject;	// position, angle, type/size, name/id, velocity
	typedef std::deque<TObject> DTObject;

	/**
	 * Holds cameras and manages them
	 */
	class Dispatcher
	{
		/** Size of maximum camera inaccuracy [m] */
		static const double CAMERA_DISTORTION_RATIO;

	public:
		typedef boost::shared_ptr<Camera> PCamera;
		typedef boost::tuple<Point, double, double, double, int> TCamera; // position, range, direction, angle, id
		typedef std::deque<TCamera> DTCamera;	

		/**
		 * Adds camera into dispatcher
		 *
		 * @param camera camera to add
		 */
		void addCamera(const PCamera & camera);

		/**
		 * Get essential information about cameras to draw them on screen
		 *
		 * @return tuple of infromation:
		 *			<position, range, direction, angle, id>
		 */
		DTCamera cameras() const;

		/**
		 * Check if there is any object in range of each camera
		 * If there is, calculate camera seeing distortion and log it out
		 *
		 * @param simulationTime time since start of the simulation in us
		 * @param objects set of objects' data, that should be checked whether in range
		 */
		void log(long long int simulationTime, const DTObject & objects);

	private:
		/** Dispatcher's cameras */
		std::deque<PCamera> cameras_;
	};
}

#endif // DISPATCHER_HPP