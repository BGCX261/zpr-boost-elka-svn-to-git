#include "Controller.hpp"

#include <boost/filesystem.hpp>
#include <iostream>
#include <cstdlib>


#ifdef TESTING
	#include "Base.hpp"
	#include <boost/test/minimal.hpp>
	int test_main(int argc, char** const)
	{
		using namespace zpr;
		BOOST_CHECK(Point(0,0) + Point(1,1) == Point(1,1));
		BOOST_CHECK(Point(0,1) + Point(1,0) == Point(1,1));
		BOOST_CHECK(Point(1,0) + Point(0,1) == Point(1,1));
		BOOST_CHECK(Point(0,0) + Point(10,10) == Point(10,10));

		Point p1(0, 0), p2(3,4);
		BOOST_CHECK(Point::distance(p1, p2) == 5.000);
		BOOST_CHECK(Point::distance(Point(1, 1), Point(4, 5)) == 5.000);
		BOOST_CHECK(Point::distance(Point(-1, -1), Point(-4, -5)) == 5.000);
		BOOST_CHECK(Point::distance(Point(5, 12), Point(0,0)) == 13.000);
		
		BOOST_CHECK(Point::angle(Point(0, 0), Point(1, 1)) > 0.785 && Point::angle(Point(0, 0), Point(1, 1)) < 0.790);

		std::cout << "\n\nWaiting for int..." << std::endl;
		int i;
		std::cin >> i;
		return EXIT_SUCCESS;
	}
#else
	int main(int, char **)
	{
		try
		{
			boost::filesystem::path applicationPath = boost::filesystem::path(__FILE__).remove_filename();
			zpr::Controller controller(applicationPath);
			controller.run();
		}
		catch(std::exception & e)
		{
			std::cout << "Wyjatek: " << e.what() << std::endl;
		}
		catch(...)
		{
			std::cout << "\n\nNieznany wyjatek symulacji. Program zakonczono gwaltownie." << std::endl;
		}

		return EXIT_SUCCESS;
	}
#endif // TESTING