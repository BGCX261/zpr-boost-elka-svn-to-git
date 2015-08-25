#ifndef EVENTS_HPP
#define EVENTS_HPP

namespace zpr
{
	class Controller;
	
	/**
	 * Event is abstract base class for user-generated events in application.
	 * Event model implements visitor pattern where Controller is visited by events. 
	 * As events should only be differentiated, they do not contain data.
	 * Derived classes are used only to call appriopriate Controller's Procees function,
	 * using polymorphic behaviour.
	 */
	struct Event
	{
		/**
		 * Calls appriopriate Controller's Process function, determined by event's type.
		 * @param c	Controller's object responsible for reacting to user events
		 */
		virtual void accept(Controller & c) = 0;
	};

	/** Event signalling desire to start simulation. */
	struct EventStart : public Event
	{
		/**
		 * Calls appriopriate Controller's Process function, determined by event's type.
		 * @param c	Controller's object responsible for reacting to user events
		 */
		virtual void accept(Controller & c);
	};

	/** Event signalling desire to stop (pause) simulation. */
	struct EventStop : public Event
	{
		/**
		 * Calls appriopriate Controller's Process function, determined by event's type.
		 * @param c	Controller's object responsible for reacting to user events
		 */
		virtual void accept(Controller & c);
	};

	/** Event signalling desire to restart simulation. */
	struct EventRestart : public Event
	{
		/**
		 * Calls appriopriate Controller's Process function, determined by event's type.
		 * @param c	Controller's object responsible for reacting to user events
		 */
		virtual void accept(Controller & c);
	};

	/** Event signalling desire to end and close simulation. */
	struct EventClose : public Event
	{
		/**
		 * Calls appriopriate Controller's Process function, determined by event's type.
		 * @param c	Controller's object responsible for reacting to user events
		 */
		virtual void accept(Controller & c);
	};

	/** Event signalling desire to make simulation run in loop (or stop this). */
	struct EventLoop : public Event
	{
		/**
		 * Calls appriopriate Controller's Process function, determined by event's type.
		 * @param c	Controller's object responsible for reacting to user events
		 */
		virtual void accept(Controller & c);
	};
}

#endif // EVENTS_HPP