#include "Dispatcher.hpp"
#include "Model.hpp"
#include "Logger.hpp"

#include <string> // debug

#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>
#include <ctime>

namespace zpr
{
	const double Dispatcher::CAMERA_DISTORTION_RATIO = 100.0;

	void Dispatcher::addCamera(const PCamera & camera)
	{
		cameras_.push_back(camera);
	}

	Dispatcher::DTCamera Dispatcher::cameras() const
	{
		DTCamera tmp;
		BOOST_FOREACH(const PCamera & camera, cameras_)
			tmp.push_back(boost::make_tuple(camera->position(), camera->range(), camera->direction(), camera->angle(), camera->id()));

		return tmp;
	}

	void Dispatcher::log(long long int simulationTime, const Model::DTObject & objects)
	{
		static boost::random::uniform_01<boost::random::mt19937> distribution(boost::random::mt19937(static_cast<unsigned int>(std::time(0))));

		BOOST_FOREACH(const PCamera & camera, cameras_)
		{
			BOOST_FOREACH(const Model::TObject & object, objects)
				if(camera->inRange(object.get<0>()))
				{
					double noise = camera->noise() * CAMERA_DISTORTION_RATIO;
					std::string msg = (boost::format("%6.3f") % (simulationTime / 1000000.0)).str();
					msg += "s Camera (" + boost::lexical_cast<std::string>(camera->id());
					msg += ") reports: " + boost::lexical_cast<std::string>(object.get<3>());
					msg += " on\t(" + (object.get<0>() + Point((distribution() - 0.5) * noise, (distribution() - 0.5) * noise)).str() + ")";
					msg += "\t{ real position: [" + object.get<0>().str() + "] }";

					Logger::getInstance().message(msg);
				}
		}
	}
	
}