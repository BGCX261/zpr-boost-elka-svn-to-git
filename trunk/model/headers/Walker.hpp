#ifndef WALKER_HPP
#define WALKER_HPP

#include "Voyager.hpp"

namespace zpr
{
	/**
	 * Class representing walkers in city.
	 */
	class Walker : public Voyager
	{
		protected:
		/** Walker name */
		const std::string name_;

		public:
		/**
		 * Walker constructor
		 *
		 * @param name walker name / id
		 * @param velocity walker velocity [km/h]
		 * @param track object track
		 */
		Walker(const std::string &name, double velocity, const PTrack & track);
		~Walker();
		
		/**
		 * Check object type (enum)
		 *
		 * @return this object type
		 */
		OBJECTS type() const;
		
		/**
		 *
		 * @return walker name / id
		 */
		const std::string& id() const;

		/**
		 * Reset walker state, back it on the beginning of track
		 */
		void reset();

		/**
		 * Next simulator iteration
		 * Walker moves depend on elapsed time
		 *
		 * @param elapsed_time time which elapsed since last update (or to simulate movement)
		 */
		void move(long long int elapsed_time);
	};
}

#endif // WALKER_HPP