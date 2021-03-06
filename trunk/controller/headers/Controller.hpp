#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "Model.hpp"
#include "View.hpp"
#include "Timer.hpp"
#include "Events.hpp"

#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/function.hpp>
#include <boost/chrono.hpp>
#include <queue>

namespace zpr
{
	/**
	 * Controller's class responsible for managing whole application according to MVC pattern.
	 * Runs and manages model, view, main timer and threads assigned to them.
	 * Processes events generated by user.
	 * Is also responsible for reading input data from configuration files. 
	 */
	class Controller
	{
		public:
		/**
		 * Prepares all simulation data, loads data from configuration files.
		 * Also prepares internal logic.
		 * @param path	path to application's working directory
		 */
		Controller(const boost::filesystem::path & path);
		
		/**
		 * Main simulation's logic function.
		 * Start all simulation procedures, then enters main processing loop and waits for events to occur.
		 * When it is about to end, closes all threads and finishes logic. Then closes and returns control to caller.
		 */
		void run();

		/**
		 * Adds user's events to queue and schedules to processing.
		 * Usually called from another thread, when scheduling is required.
		 * @param newEvent	new Event to be added do queue
		 */
		void scheduleEvent(const boost::shared_ptr<Event> & newEvent);	// add new event to queue

		/** Processes request to start simulation */
		void Process(const EventStart&);
		/** Processes request to stop (pause) simulation */
		void Process(const EventStop&);
		/** Processes request to restart simulation */
		void Process(const EventRestart&);
		/** Processes request to end and close simulation */
		void Process(const EventClose&);
		/** Processes request to enable looping simulation */
		void Process(const EventLoop&);

		private:
		/** Starts threads required by simulation to run. */
		void runThreads();
		/** Interrupts and joins threads started by runThreads(). */
		void endThreads();
	
		/**
		 * Responsible for loading loading data about map from configuration file.
		 * @param path	path to application's working directory
		 */
		void parseMap(const boost::filesystem::path & path);
		/**
		 * Responsible for loading loading data about cameras from configuration file.
		 * @param path	path to application's working directory
		 */
		void parseDispatcher(const boost::filesystem::path & path);
		/**
		 * Responsible for loading loading data about voyagers from configuration file.
		 * @param path	path to application's working directory
		 */
		void parseObjects(const boost::filesystem::path & path);
				
		/** Contains model assiociated data. */
		Model model_;
		/** Responsible for presenting model_ data graphically. */
		View view_;
		/** Main application's timer responsible for generating all periodic events. */
		Timer timer_;
		/** Model's thread */
		boost::thread modelThread;
		/** View's thread */
		boost::thread viewThread;
		/** Timer's thread */
		boost::thread timerThread;
		
		/** Thread synchronization object, used for notification from other threads. */
		boost::condition_variable eventCondition_;
		/** User-generated events queue. Events scheduled for processing, but not processed yet.*/
		std::queue<boost::shared_ptr<Event> > eventQueue_;
		/** Mutex used to secure acces to eventCondition_ and eventQueue_.*/
		boost::mutex eventMutex_;

		/** Indicates whether simulation is runned. */
		bool run_;
	};
}
#endif // CONTROLLER_HPP