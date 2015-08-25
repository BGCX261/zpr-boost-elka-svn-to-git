#include "Walker.hpp"
#include "Track.hpp"
#include <iostream>
#include <string>

namespace zpr
{
	Walker::Walker(const std::string &name, double velocity, const PTrack & track) : Voyager(track), name_(name)
	{
		velocity_ = velocity;
	}

	Walker::~Walker() {}
	
	OBJECTS Walker::type() const
	{
		return WALKER;
	}

	const std::string& Walker::id() const
	{
		return name_;
	}

	void Walker::reset()
	{
		finished_ = false;
		percentDistanceTraveled_ = 0;
		position_ = track_->start();
	}

	void Walker::move(long long int elapsed_time)
	{
		double dt = (double) elapsed_time / 1000;

		Point prev_position = position_;

		percentDistanceTraveled_ += (velocity_ * dt) / track_->length();
		position_ = track_->positionOnTrack( percentDistanceTraveled_ ).first;

		angle_ = Point::angle(prev_position, position_);

		if(percentDistanceTraveled_ > 1.0)
			finished_ = true;
	}
}