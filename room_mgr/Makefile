ifeq ($(version),debug)
flag=-g -rdynamic
else
flag=-g -rdynamic 
endif

#»ù´¡.oµÄÂ·¾¶
CommonObj=$(wildcard ../game_common/common/obj/*.o)

CommonDir=$(wildcard ../game_common/common/*.hpp)

IncDir=$(wildcard ../game_common/*.hpp)

room_mgr_depends= room_mgr.o room_mgr_main.o room_mgr_cmd.o 
room_mgr:$(room_mgr_depends)
	g++ -o room_mgr $(room_mgr_depends) $(CommonObj) -lpthread -lwebsockets -lcurl

room_mgr.o:room_mgr.cpp room_mgr.hpp $(IncDir)
	g++ -c room_mgr.cpp room_mgr.hpp $(IncDir)

room_mgr_main.o: room_mgr_main.cpp $(IncDir)
	g++ -c room_mgr_main.cpp $(IncDir)

room_mgr_cmd.o: room_mgr_cmd.cpp room_mgr_cmd.hpp room_mgr.hpp $(IncDir) $(CommonDir)
	g++ -c room_mgr_cmd.cpp room_mgr_cmd.hpp room_mgr.hpp $(IncDir) $(CommonDir)

clean:
	-rm *.o
	-rm *.gch
	-rm room_mgr
	-rm ../game_common/*.gch