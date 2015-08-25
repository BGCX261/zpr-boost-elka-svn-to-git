#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <boost/thread/mutex.hpp>

namespace zpr
{
	/**
	 * Singleton logger to write essential
	 * information about simulation into file
	 * or standard output
	 */
	class Logger
	{
		/** Message counter */
		mutable int count_;
		/** Output log stream */
		std::ostream *output_;
		/** Input Output control mutex */
		mutable boost::mutex ioMutex_;
		/** Standard error stream control mutex */
		mutable boost::mutex stderrMutex_;

		public:
			
		/**
		 * @return Logger instance
		 */
		static Logger& getInstance();
		
		/**
		 * Logs message into file or if IOException
		 * (in constructor) onto user stdout
		 *
		 * @param msg message to log
		 */
		void message(const char *msg) const;
		
		/**
		 * Overloaded, do the same as other message
		 *
		 * @param msg message to log
		 */
		void message(const std::string &msg) const;

		/**
		 * Logs error message into standard error stream
		 * @param msg error message to log
		 */
		void error(const char *msg) const;

		/**
		 * Logs error message into standard error stream
		 * @param msg error message to log
		 */
		void error(const std::string &msg) const;

		private:
			
		/**
		 * Private constructor to make only one
		 * instance of logger - Singleton
		 *
		 * Creates file named as ${actual timestamp}.txt
		 * if succeeded write log there, otherwise write onto std::cout
		 */
		Logger();
		Logger(const Logger &other);
		const Logger& operator=(const Logger &other);
		
		/**
		 * Close file stream and delete handle
		 */
		~Logger();
	};
}

#endif // LOGGER_HPP