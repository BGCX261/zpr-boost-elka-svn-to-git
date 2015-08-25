HDRS= -I model/headers -I view/headers -I controller/headers
LIBS= -l boost_filesystem -l boost_thread -l boost_chrono -l boost_date_time
ALLEGRO=`pkg-config --libs --static allegro-static-5.1 allegro_primitives-static-5.1 allegro_font-static-5.1 allegro_image-static-5.1`
OBJS=main.o
OBJSM=model/Base.o model/Dispatcher.o model/Model.o model/Track.o model/Walker.o model/Graph.o model/Segment.o model/Vehicle.o model/Camera.o model/Voyager.o
OBJSV=view/View.o
OBJSC=controller/Controller.o controller/Events.o controller/Logger.o controller/Timer.o
MACROS=

all: $(OBJS) $(OBJSM) $(OBJSV) $(OBJSC)
	@echo "\t\tWait for a while... Let me finish it"
	@g++ -Wall $^ $(CFLAGS) $(HDRS) $(LIBS) $(ALLEGRO) $(MACROS) -o $@

.cpp.o:
	@echo "\t$< => .o"
	@g++ -Wall $(CFLAGS) $(HDRS) $(LIBS) $(ALLEGRO) $(MACROS) -c $*.cpp -o $*.o

test:
	@make all MACROS="-D TESTING"

clean:
	@rm -rf *.o 2>/dev/null
	@rm -rf model/*.o 2>/dev/null
	@rm -rf view/*.o 2>/dev/null
	@rm -rf controller/*.o 2>/dev/null

model/Base.o: model/headers/Base.hpp
model/Dispatcher.o: model/headers/Dispatcher.hpp model/headers/Model.hpp controller/headers/Logger.hpp model/headers/Camera.hpp model/headers/Voyager.hpp
model/Model.o: model/headers/Model.hpp controller/headers/Logger.hpp model/headers/Dispatcher.hpp model/headers/Vehicle.hpp model/headers/Walker.hpp model/headers/Graph.hpp
model/Track.o: model/headers/Track.hpp model/headers/Segment.hpp
model/Walker.o: model/headers/Walker.hpp model/headers/Track.hpp model/headers/Voyager.hpp
model/Graph.o: model/headers/Graph.hpp model/headers/Base.hpp
model/Segment.o: model/headers/Segment.hpp model/headers/Base.hpp
model/Vehicle.o: model/headers/Vehicle.hpp model/headers/Track.hpp model/headers/Voyager.hpp
model/Camera.o: model/headers/Camera.hpp model/headers/Base.hpp model/headers/Voyager.hpp
model/Voyager.o: model/headers/Voyager.hpp model/headers/Base.hpp model/headers/Track.hpp

view/View.o: controller/headers/Controller.hpp view/headers/View.hpp controller/headers/Logger.hpp model/headers/Model.hpp model/headers/Base.hpp

controller/Controller.o: controller/headers/Controller.hpp controller/headers/Logger.hpp model/headers/Walker.hpp model/headers/Voyager.hpp model/headers/Track.hpp controller/headers/Timer.hpp model/headers/Graph.hpp model/headers/Model.hpp view/headers/View.hpp controller/headers/Events.hpp
controller/Events.o: controller/headers/Events.hpp controller/headers/Controller.hpp 
controller/Logger.o: controller/headers/Logger.hpp
controller/Timer.o: controller/headers/Timer.hpp controller/headers/Logger.hpp
