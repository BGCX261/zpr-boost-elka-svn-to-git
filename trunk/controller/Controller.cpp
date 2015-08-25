#include "Controller.hpp"
#include "Walker.hpp"
#include "Voyager.hpp"
#include "Track.hpp"
#include "Graph.hpp"
#include "Timer.hpp"
#include "Logger.hpp"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

namespace zpr
{
	Controller::Controller(const boost::filesystem::path & path) : view_(*this, model_), run_(true)
	{
		std::string paths[3] = { "xml_data/dispatcher.xml", "xml_data/streets.xml", "xml_data/objects.xml" };
		std::string mesgs[3] = { "Retype path (ex. '", "') or type 'q' to exit:\n\t", "No matching path like: " };
		
		for (int i = 0; i < 3; ++i)
		{
			while (!boost::filesystem::exists(path / paths[i]))
			{
				std::cout << mesgs[2] << paths[i] << std::endl;
				std::cout << mesgs[0] << paths[i] << mesgs[1];
				std::cin >> paths[i];

				if(paths[i] == "q")
					throw(std::runtime_error(("Quit without starting application while trying to load " + paths[i]).c_str()));
			}
		}

		parseDispatcher(path / paths[0]);
		parseMap(path / paths[1]);
		parseObjects(path / paths[2]);

		model_.start();
		
		timer_.AddListener(TimerListener(boost::bind(&Model::scheduleUpdate, &model_, _1, _2), boost::chrono::milliseconds(10)));
		timer_.AddListener(TimerListener(boost::bind(&View::scheduleRefresh, &view_, _1, _2), boost::chrono::milliseconds(10)));
		timer_.AddListener(TimerListener(boost::bind(&Model::scheduleLog, &model_, _1, _2), boost::chrono::milliseconds(1000)));
	}
	
	void Controller::scheduleEvent(const boost::shared_ptr<Event> & newEvent)
	{
		{
			boost::lock_guard<boost::mutex> lock(eventMutex_);
			eventQueue_.push(newEvent);
		}
		eventCondition_.notify_one();
	}
	
	void Controller::runThreads()
	{
		viewThread = boost::thread(boost::ref(view_));
		modelThread = boost::thread(boost::ref(model_));
		timerThread = boost::thread(boost::ref(timer_));
	}

	void Controller::endThreads()
	{
		viewThread.interrupt();
		timerThread.interrupt();
		modelThread.interrupt();
		
		viewThread.join();
		timerThread.join();
		modelThread.join();
	}

	void Controller::run()
	{
		try
		{
			runThreads();
			while(run_ && !boost::this_thread::interruption_requested())
			{
			
				boost::unique_lock<boost::mutex> lock(eventMutex_);
				while(eventQueue_.empty())
					eventCondition_.wait(lock);
					
				boost::shared_ptr<Event> ev = eventQueue_.front();
				eventQueue_.pop();
				ev->accept(*this);
			}
		}
		catch(boost::thread_interrupted)
		{
			// thread interrupted
			// closes gracefully
		}
		catch(...)
		{
			Logger::getInstance().error("Unknown Controller exception.");
		}
		endThreads();
	}

	void Controller::Process(const EventStart&)
	{
		model_.setActive(true);
	}

	void Controller::Process(const EventStop&)
	{
		model_.setActive(false);
	}

	void Controller::Process(const EventRestart&)
	{
		model_.start();
		view_.scheduleRefresh(0, 0);
	}

	void Controller::Process(const EventClose&)
	{
		run_ = false;
	}

	void Controller::Process(const EventLoop&)
	{
		model_.switchLoop();
	}

	void Controller::parseDispatcher(const boost::filesystem::path & path)
	{
		using boost::property_tree::ptree;
		ptree pt;
	
		try
		{
			read_xml(path.string(), pt, boost::property_tree::xml_parser::no_comments);
			BOOST_FOREACH(ptree::value_type &v, pt.get_child("dispatcher"))
			{
				// get cam parameteres
				int id = v.second.get<int>("id");
				Point position(v.second.get<double>("position.<xmlattr>.x"), v.second.get<double>("position.<xmlattr>.y"));
				double direction = v.second.get<double>("direction");
				double angle = v.second.get<double>("angle");
				double range = v.second.get<double>("range");
				double precision = v.second.get<double>("precision");

				// add camera
				model_.addCamera(Dispatcher::PCamera(new Camera(id, position, direction, angle, range, precision)));
			}
		}
		catch (boost::property_tree::ptree_bad_path &e)
		{
			throw std::runtime_error((std::string(e.what()) + " : in file (" + path.string() + ")").c_str());
		}
	}

	void Controller::parseMap(const boost::filesystem::path & path)
	{
		using boost::property_tree::ptree;
		ptree pt;

		try
		{
			read_xml(path.string(), pt, boost::property_tree::xml_parser::no_comments);
			Model::PGraph graph(new Graph());

			BOOST_FOREACH( ptree::value_type &v, pt.get_child("map") )
			{
				if ( v.first == "nodes" )
					BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("") )
					{
						Point position( vv.second.get<double>("<xmlattr>.x"), vv.second.get<double>("<xmlattr>.y") );
						graph->addVertex(vv.second.get<int>(""), position);
					}
				else if ( v.first == "edges" )
					{
					BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("") )
						graph->addEdge(vv.second.get<int>("<xmlattr>.from"), vv.second.get<int>("<xmlattr>.to"));
					}
			}

			model_.streets(graph);
		}
		catch (boost::property_tree::ptree_bad_path &e)
		{
			throw std::runtime_error((std::string(e.what()) + " : in file (" + path.string() + ")").c_str());
		}
	}

	void Controller::parseObjects(const boost::filesystem::path & path)
	{
		using boost::property_tree::ptree;
		ptree pt;
	
		try
		{
			read_xml(path.string(), pt, boost::property_tree::xml_parser::no_comments);
			BOOST_FOREACH(ptree::value_type &v, pt.get_child("objects"))
			{
				if (v.second.get<std::string>("<xmlattr>.type") == "car")
				{
					std::string id = v.second.get<std::string>("id");
					double acceleration = v.second.get<double>("parameters.acceleration");
					double weight = v.second.get<double>("parameters.weight");
					std::string size = v.second.get<std::string>("parameters.size");
					double mspeed = v.second.get<double>("parameters.maxspeed");
					
					Voyager::PTrack track(new VehicleTrack());
					BOOST_FOREACH(ptree::value_type &vv, v.second.get_child("track"))
						track->addPoint(Point(vv.second.get<double>("<xmlattr>.x"), vv.second.get<double>("<xmlattr>.y")));

					if(size == "big")
						model_.addObject(Model::PVoyager(new BigCar(id, acceleration, weight, mspeed, track)));
					else
						model_.addObject(Model::PVoyager(new SmallCar(id, acceleration, weight, mspeed, track)));
				}
				else if ( v.second.get<std::string>("<xmlattr>.type") == "walker" )
				{
					std::string id = v.second.get<std::string>("id");
					double mspeed = v.second.get<double>("parameters.maxspeed");

					Voyager::PTrack track(new WalkerTrack());
					BOOST_FOREACH(ptree::value_type &vv, v.second.get_child("track"))
						track->addPoint(Point(vv.second.get<double>("<xmlattr>.x"), vv.second.get<double>("<xmlattr>.y")));

					model_.addObject(Model::PVoyager(new Walker(id, mspeed, track)));
				}
			}
		}
		catch (boost::property_tree::ptree_bad_path &e)
		{
			throw std::runtime_error((std::string(e.what()) + " : in file (" + path.string() + ")").c_str());
		}
	}
}