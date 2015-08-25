#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Voyager.hpp"

namespace zpr
{
	/**
	 * Class representing vechicles in city.
	 */
	class Vehicle : public Voyager
	{
		protected:
		/** Vehicle registration */
		const std::string registration_;
		/** Vehicle acceleration [m/s] */
		double acceleration_;
		/** Vehicle weight [kg] */
		double weight_;
		/** Vehicle maximum velocity [m/s] */
		double maxSpeed_;
		/** Breaking flag */
		bool break_;

		public:
		/**
		 * Vehicle constructor
		 *
		 * @param id vehicle registration
		 * @param acceleration vehicle acceleration [m/s]
		 * @param weight vehicle weight [kg]
		 * @param maxSpeed vehicle maximum velocity [km/h]
		 * @param track object track
		 */
		Vehicle(const std::string &id, double acceleration, double weight, double maxSpeed, const PTrack & track);
		
		/**
		 *
		 * @return vehicle registration
		 */
		const std::string& id() const;

		/**
		 * Reset vehicle state, back it on the beginning of track
		 */
		void reset();
		
		/**
		 * Next simulator iteration
		 * Vehicle moves depend on elapsed time
		 *
		 * @param elapsed_time time which elapsed since last update (or to simulate movement)
		 */
		void move(long long int elapsed_time);
	};

	/**
	 * Class representing small car in simulation
	 */
	class SmallCar: public Vehicle
	{
		public:
		/**
		 * Construct small car object
		 * 
		 * @param id cars registration
		 * @param acceleration cars acceleration
		 * @param weight cars weight
		 * @param maxSpeed maximum cars velocity
		 * @param track object track
		 */
		SmallCar(std::string &id, double acceleration, double weight, double maxSpeed, const PTrack & track);
		
		/**
		 * Check object type (enum)
		 *
		 * @return this object type
		 */
		OBJECTS type() const;
	};

	/**
	 * Class representing big car in simulation
	 */
	class BigCar: public Vehicle
	{
		public:
		/**
		 * Construct big car object
		 * 
		 * @param id cars registration
		 * @param acceleration cars acceleration
		 * @param weight cars weight
		 * @param maxSpeed maximum cars velocity
		 * @param track object track
		 */
		BigCar(const std::string &id, double acceleration, double weight, double maxSpeed, const PTrack & track);
		
		/**
		 * Check object type (enum)
		 *
		 * @return this object type
		 */
		OBJECTS type() const;
	};
}

#endif // VEHICLE_HPP