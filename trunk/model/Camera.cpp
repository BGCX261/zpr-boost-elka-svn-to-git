#include "Camera.hpp"

namespace zpr
{
	Camera::Camera(int id, Point position, double direction, double angle, double range, double precision)
		: id_(id), position_(position), direction_(direction * 3.141 / 180.0), angle_(angle * 3.141 / 180.0), range_(range), precision_(precision)
	{
	}

	int Camera::id() const
	{
		return id_;
	}
	
	double Camera::range() const
	{
		return range_;
	}
	
	double Camera::angle() const
	{
		return angle_;
	}
	
	double Camera::direction() const
	{
		return direction_;
	}

	const Point Camera::position() const
	{
		return position_;
	}

	bool Camera::inRange(const Point & object) const
	{
		if(range_ >= Point::distance(position_, object))
			if(zpr::inRange(direction_ - angle_ / 2.0, direction_ + angle_ / 2.0, Point::angle(position_, object)))
				return true;
		return false;
	}

	double Camera::noise() const
	{
		return 1.0 - precision_;
	}
}