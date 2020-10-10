ifeq ($(version),debug)
flag=-g -rdynamic
else
flag=-g -rdynamic 
endif

#»ù´¡.oµÄÂ·¾¶
CommonObj=$(wildcard ../game_common/common/obj/*.o)

CommonDir=$(wildcard ../game_common/common/*.hpp)

IncDir=$(wildcard ../game_common/*.hpp)

server_depends= server.o server_cmd.o server_event.o server_main.o \
server_tcp_server.o server_websocket.o
server:$(server_depends)
	g++ -o server $(server_depends) $(CommonObj) -lpthread -lwebsockets -lcurl

server.o:server.cpp server.hpp
	g++ -c server.cpp server.hpp $(CommonDir) $(IncDir)

server_main.o:server_main.cpp server_event.hpp
	g++ -c server_main.cpp server_event.hpp $(CommonDir) $(IncDir)

server_cmd.o:server_cmd.cpp
	g++ -c server_cmd.cpp $(CommonDir) $(IncDir)

server_event.o:server_event.cpp server_event.hpp
	g++ -c server_event.cpp server_event.hpp $(CommonDir) $(IncDir)

server_tcp_server.o:server_tcp_server.cpp 
	g++ -c server_tcp_server.cpp $(CommonDir) $(IncDir)

server_websocket.o:server_websocket.cpp
	g++ -c server_websocket.cpp $(CommonDir) $(IncDir)

clean:
	-rm *.o
	-rm *.gch
	-rm server
	-rm ../game_common/*.gch