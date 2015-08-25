#include "Voyager.hpp"

namespace zpr
{
	Voyager::Voyager(const PTrack & track) : position_(Point(0, 0)), velocity_(0), angle_(0), percentDistanceTraveled_(0), track_(track), finished_(false)
	{
		position_ = track_->start();
	}

	/**
	 * Empty abstract class' destructor
	 */
	Voyager::~Voyager() {}

	const Point& Voyager::position() const
	{
		return position_;
	}

	double Voyager::angle() const
	{
		return angle_;
	}

	double Voyager::velocity() const
	{
		return velocity_;
	}

	bool Voyager::finished()
	{
		return finished_;
	}
}