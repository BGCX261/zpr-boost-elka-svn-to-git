#ifndef TEST111
#define TEST111

#ifdef TESTING
	#include <boost/test/minimal.hpp>
#else
	//#define BOOST_ERROR
#endif // TESTING
#endif // TEST111