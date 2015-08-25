#include "Events.hpp"
#include "Controller.hpp"

namespace zpr
{
	void EventStart::accept(Controller & c)
	{
		c.Process(*this);
	}

	void EventStop::accept(Controller & c)
	{
		c.Process(*this);
	}

	void EventRestart::accept(Controller & c)
	{
		c.Process(*this);
	}

	void EventClose::accept(Controller & c)
	{
		c.Process(*this);
	}

	void EventLoop::accept(Controller & c)
	{
		c.Process(*this);
	}
}