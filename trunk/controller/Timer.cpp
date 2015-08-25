#include "Timer.hpp"
#include "Logger.hpp"

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

namespace zpr
{
	TimerListener::TimerListener(const boost::function<void (long long int, long long int)> & listener, const Duration & frequency) : listener_(listener), frequency_(frequency)
	{}

	bool TimerListener::check(const Duration & simulationTime)
	{
		if((simulationTime - previousTrigger_) > frequency_)
		{
			listener_(boost::chrono::duration_cast<boost::chrono::microseconds>(simulationTime).count(), boost::chrono::duration_cast<boost::chrono::microseconds>(simulationTime - previousTrigger_).count());
			previousTrigger_ = simulationTime;
			return true;
		}
		return false;
	}

	Timer::Timer() : started_(now())
	{}
	
	Duration Timer::simulationTime() const
	{
		return now() - started_;
	}

	void Timer::AddListener(const TimerListener & listener)
	{
		listeners_.push_back(listener);
	}

	TimePoint Timer::now() const
	{
		return boost::chrono::high_resolution_clock::now();
	}

	void Timer::operator()()
	{
		static const Duration TICK = boost::chrono::milliseconds(1);
		static const boost::posix_time::time_duration SLEEP_TIME = boost::posix_time::milliseconds(1);
		try
		{
			while(!boost::this_thread::interruption_requested())
			{
				TimePoint prev = now();
				TimePoint nextTick = prev + TICK;
				while(nextTick > now())
				{
					boost::this_thread::yield();
					boost::this_thread::sleep(SLEEP_TIME);
				}
				
				std::for_each(listeners_.begin(), listeners_.end(), boost::bind(&TimerListener::check, _1, simulationTime()));
			}
		}
		catch(boost::thread_interrupted)
		{
			// thread interrupted
			// closes gracefully
		}
		catch(...)
		{
			Logger::getInstance().error("Unknown Timer exception.");
		}
	}
}