#include "Track.hpp"
#include <math.h>

namespace zpr
{
	const double VehicleTrack::BEZIER_DIVISION = 10;

	Track::Track() : length_(0.0)
	{
	}

	Track::~Track() {}
	
	Point Track::start()
	{
		return segments_.empty() ? Point() : segments_.front()->begin();
	}

	Point Track::finish()
	{
		return segments_.empty() ? Point() : segments_.back()->end();
	}

	double Track::length()
	{
		return length_;
	}

	std::pair<Point, double> Track::positionOnTrack(double percent)
	{
		if ( percent < 0 )
			percent = 0;
		else if ( percent > 1 )
			percent = 1;

		double traveled_length = length_ * percent;
		double temp_length = 0;

		for (DSegments::const_iterator it = segments_.begin(); it != segments_.end(); ++it)
		{
			temp_length += (*it)->length();

			if ( traveled_length <= temp_length )
			{
				return std::make_pair((*it)->position( 1 - ( ( temp_length - traveled_length ) / (*it)->length() ) ), (*it)->bezier());
			}
		}

		return std::make_pair(Point(), 0);
	}

	std::pair<double, double> Track::nextBezierDistanceAngle(double percent)
	{
		if ( percent < 0 )
			percent = 0;
		else if ( percent > 1 )
			percent = 1;
		
		double traveled_length = length_ * percent;
		double temp_length = 0;

		for (DSegments::const_iterator it = segments_.begin(); it != segments_.end(); ++it)
		{
			temp_length += (*it)->length();

			if ( traveled_length <= temp_length && (*it)->bezier() )
			{
				return std::make_pair((temp_length - (*it)->length()) / length_, (*it)->bezier());
			}
		}

		return std::make_pair(2, 0);
	}

	void Track::recalculateLength()
	{
		length_ = 0;

		for (DSegments::const_iterator it = segments_.begin(); it != segments_.end(); ++it)
			length_ += (*it)->length();
	}

	void VehicleTrack::addPoint(Point point)
	{
		if (segments_.empty())
			segments_.push_back( PSegment( new StraightSegment( point, point ) ) );
		else if ( segments_.size() == 1 && segments_.front()->begin() == segments_.front()->end() )
			segments_.front().reset( new StraightSegment( segments_.front()->begin(), point ) );
		else
		{
			if ( segments_.back()->end() == point )
				return;
			// Mowiac krotko jak jest jeden segment to jest StraightSegment
			// jak dodajemy trzeci punkt to juz mozna beziera zrobic, wiec jest : straight -> bezier -> straight
			// jak kolejny dajemy to jest : straight -> bezier -> straight(mozliwe ze o zerowej dlugosci jak nie wyrabia sie zakretowo) -> bezier -> straight

			Point realBegin = segments_.back()->begin();
			Point lastBegin = realBegin;
			Point lastEnd = segments_.back()->end();
			
			segments_.pop_back();
			
			if (!segments_.empty())
				realBegin = static_cast<BezierSegment*>(segments_.back().get())->control();

			Point p1, p2;
			std::pair<Point, Point> p_tmp = bothBezierBetween(lastBegin, point, lastEnd);
			p1 = p_tmp.first;
			p2 = p_tmp.second;

			// optymalizacja(jeszcze nie ma :P): jak begin i end na tym samym x'ie lub y'ku to tylko straight
			segments_.push_back( PSegment( new StraightSegment( lastBegin, p1 ) ) );
			segments_.push_back( PSegment( new BezierSegment( p1, p2, lastEnd ) ) );
			segments_.push_back( PSegment( new StraightSegment( p2, point ) ) );
		}

		recalculateLength();
	}

	std::pair<Point, Point> VehicleTrack::bothBezierBetween(const Point &start, const Point &final, const Point &control)
	{
		std::pair<Point, Point> tmp = std::make_pair(control, control);
		double p1_distance = Point::distance(control, start);
		double p2_distance = Point::distance(control, final);

		if ( p1_distance <= 2 * BEZIER_DIVISION || p2_distance <= 2 * BEZIER_DIVISION )
		{
			double min_distance = ( p1_distance < p2_distance ? p1_distance : p2_distance ) / 2;

			tmp.first.x_ += (start.x_ - control.x_) * ( min_distance / p1_distance );
			tmp.first.y_ += (start.y_ - control.y_) * ( min_distance / p1_distance );
			tmp.second.x_ += (final.x_ - control.x_) * ( min_distance / p2_distance );
			tmp.second.y_ += (final.y_ - control.y_) * ( min_distance / p2_distance );
		}
		else
		{
			tmp.first.x_ += (start.x_ - control.x_) * ( BEZIER_DIVISION / p1_distance );
			tmp.first.y_ += (start.y_ - control.y_) * ( BEZIER_DIVISION / p1_distance );
			tmp.second.x_ += (final.x_ - control.x_) * ( BEZIER_DIVISION / p2_distance );
			tmp.second.y_ += (final.y_ - control.y_) * ( BEZIER_DIVISION / p2_distance );
		}

		return tmp;
	}

	void WalkerTrack::addPoint(Point point)
	{
		if(segments_.empty())
			segments_.push_back(PSegment(new StraightSegment(point, point)));
		else if(segments_.size() == 1 && segments_.front()->begin() == segments_.front()->end())
			segments_.front().reset(new StraightSegment(segments_.front()->begin(), point));
		else
			segments_.push_back(PSegment(new StraightSegment(segments_.back()->end(), point)));

		recalculateLength();
	}
}