#include "Segment.hpp"
#include <cmath>

namespace zpr
{
	/************************
	*	Segment				*
	************************/
	Segment::Segment(const Point begin, const Point end) : begin_(begin), end_(end)
	{
	}

	Segment::~Segment() {}
	
	Point Segment::begin()
	{
		return begin_;
	}
	
	Point Segment::end()
	{
		return end_;
	}

	/************************
	*	BezierSegment		*
	*************************/
	BezierSegment::BezierSegment(const Point begin, const Point end, const Point control) : Segment(begin, end), control_(control)
	{
	}

	Point BezierSegment::position(double percent)
	{
		if ( percent < 0 )
		{
			percent = 0;
		}
		else if ( percent > 1 )
		{
			percent = 1;
		}

		Point tmp;
		tmp.x_ = (1 - percent) * (1 - percent) * begin_.x_ + 2 * (1 - percent) * percent * control_.x_ + percent * percent * end_.x_;
		tmp.y_ = (1 - percent) * (1 - percent) * begin_.y_ + 2 * (1 - percent) * percent * control_.y_ + percent * percent * end_.y_;

		return tmp;
	}

	double BezierSegment::length()
	{
		double length = 0;
		double epsilon = 0.001;

		// bo ta calka jak prosta jets to daje -1.INF to wtedy 'przyblizamy' odcinkiem
		if (Point::distance(begin_, control_) + Point::distance(control_, end_) <= Point::distance(begin_, end_) + epsilon)
		{
			length = Point::distance(begin_, end_);
		}
		else
		{
			Point a, b;
			a.x_ = begin_.x_ - 2 * control_.x_ + end_.x_;
			a.y_ = begin_.y_ - 2 * control_.y_ + end_.y_;
			b.x_ = 2 * control_.x_ - 2 * begin_.x_;
			b.y_ = 2 * control_.y_ - 2 * begin_.y_;
			double A = 4 * ( a.x_ * a.x_ + a.y_ * a.y_ );
			double B = 4 * ( a.x_ * b.x_ + a.y_ * b.y_ );
			double C = b.x_ * b.x_ + b.y_ * b.y_;

			double Sabc	= 2 * sqrt( A + B + C );
			double A_2	= sqrt(A);
			double A_32	= 2 * A * A_2;
			double C_2	= 2 * sqrt(C);
			double BA	= B / A_2;

			length = ( A_32 * Sabc + A_2 * B * ( Sabc - C_2 ) + ( 4 * C * A - B * B ) * log( ( 2 * A_2 + BA + Sabc ) / ( BA + C_2 ) ) ) / ( 4 * A_32 );
		}

		return length;
	}

	Point BezierSegment::control()
	{
		return control_;
	}

	double BezierSegment::bezier()
	{
		Point between = begin_;
		between.x_ += (end_.x_ - begin_.x_) / 2;
		between.y_ += (end_.y_ - begin_.y_) / 2;
		double angle = asin(Point::distance(between, begin_) / Point::distance(control_, begin_)) * 2;

		return angle;
	}


	/************************
	*	StraightSegment		*
	*************************/
	StraightSegment::StraightSegment(const Point begin, const Point end) : Segment(begin, end)
	{
	}

	Point StraightSegment::position(double percent)
	{
		if ( percent < 0 )
		{
			percent = 0;
		}
		else if ( percent > 1 )
		{
			percent = 1;
		}

		Point tmp(begin_.x_, begin_.y_);
		
		tmp.x_ += (end_.x_ - begin_.x_) * percent;
		tmp.y_ += (end_.y_ - begin_.y_) * percent;

		return tmp;
	}
	
	double StraightSegment::length()
	{
		return Point::distance(begin_, end_);
	}

	double StraightSegment::bezier()
	{
		return 0;
	}
}