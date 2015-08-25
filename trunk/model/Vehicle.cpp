#include "Vehicle.hpp"
#include "Track.hpp"
#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>

namespace zpr
{
	Vehicle::Vehicle(const std::string &id, double acceleration, double weight, double maxSpeed, const PTrack & track)
		: Voyager(track), registration_(id), acceleration_(acceleration), weight_(weight), maxSpeed_(maxSpeed * (10.0 / 36.0)), break_(false)
	{
	}

	const std::string& Vehicle::id() const
	{
		return registration_;
	}
	
	void Vehicle::reset()
	{
		break_ = false;
		finished_ = false;
		velocity_ = 0;
		percentDistanceTraveled_ = 0;
		position_ = track_->start();
	}

	void Vehicle::move(long long int elapsed_time)
	{
		double dt = (double) elapsed_time / 1000;

		double on_bezier = track_->positionOnTrack( percentDistanceTraveled_ ).second;
		



		// M Testy
		/*double tprzysp = maxSpeed_ / acceleration_;

		double testTick = (velocity_ * dt) / track_->length();
		Point testPosition_ = track_->positionOnTrack(testTick).first;
		
		double testAngle = Point::angle(position_, testPosition_);
		if(testAngle < 0.01)
			registration_ = "ZERO";
		else
			registration_ = boost::lexical_cast<std::string>(testAngle);//"zakret";*/
		//



		if (!break_ && !on_bezier)
		{
			// Vk -> max predkosc na bezierze
			// Vk = Vp - a*t
			// t = (Vp - Vk) / a
			
			// tu mam procent drogi oznaczajacy poczatek beziera i kat zakretu (ostrzejszy zakret -> mniejszy kat)
			std::pair<double, double> nextangle = track_->nextBezierDistanceAngle(percentDistanceTraveled_);

			double max_velocity_on_bezier = maxSpeed_ * (nextangle.second / 3.141); // % zakretu % maxSpeeda - do tylu ma zwolnic
			// licze tu czas jak dlugo hamuje z liniowy hamowaniem- > hamuje tak szybko jak przyspiesza
			double breaking_time = (velocity_ - max_velocity_on_bezier) / acceleration_;

			if (velocity_ > max_velocity_on_bezier)
			{
				double percent = percentDistanceTraveled_;

				// procent trasy w ktorym wyhamuje do tej wyzej zadanej predkosci pt. max_velocity_on...
				percent += (acceleration_ * breaking_time * breaking_time / 2) / track_->length();

				// jak ten procent jest wiekszy od poczatku beziera to zaczyna hamowac
				if (percent >= nextangle.first)
				{
					break_ = true;
				}
			}
		}

		// nie jest na bezierze i nie hamuje to zwieksza
		if (!on_bezier && !break_)
		{
			velocity_ += acceleration_ * dt;
		} // nie jest na bezierze i hamuje to zmniejsza predkosc
		else if (!on_bezier && break_)
		{
			velocity_ -= acceleration_ * dt;
		} // na bezierze nie hamuj juz, tylko jedzie stala predkoscia
		else if (on_bezier)
		{
			break_ = false;
		}

		if (velocity_ > maxSpeed_)
			velocity_ = maxSpeed_;
		if (velocity_ <= 0) {
			velocity_ = 0.001;
			break_ = false;
		}

		Point prev_position = position_;

		percentDistanceTraveled_ += ( velocity_ * dt ) / track_->length();
		position_ = track_->positionOnTrack( percentDistanceTraveled_ ).first;
		
		angle_ = Point::angle(prev_position, position_);

		if(percentDistanceTraveled_ > 1.0)
			finished_ = true;
	}

	SmallCar::SmallCar(std::string &id, double acceleration, double weight, double maxSpeed, const PTrack & track) : Vehicle(id, acceleration, weight, maxSpeed, track)
	{
	}
	
	OBJECTS SmallCar::type() const
	{
		return SMALLCAR;
	}

	BigCar::BigCar(const std::string &id, double acceleration, double weight, double maxSpeed, const PTrack & track) : Vehicle(id, acceleration, weight, maxSpeed, track)
	{
	}
	
	OBJECTS BigCar::type() const
	{
		return BIGCAR;
	}
}