ifeq ($(version),debug)
flag=-g -rdynamic
else
flag=-g -rdynamic 
endif
all:cJSON.o CJsonObject.o common_api.o tcp_client.o tcp_server.o thread_pool.o websocket_server.o
	echo "all obj is compiled"
	-mv *.o obj/
	-rm *.gch
cJSON.o: cJSON.c cJSON.h
	gcc $(flag) -c cJSON.c cJSON.h

CJsonObject.o:CJsonObject.cpp CJsonObject.hpp cJSON.h
	g++ $(flag) -c CJsonObject.cpp CJsonObject.hpp cJSON.h

common_api.o:common_api.cpp common_api.hpp
	g++ $(flag) -c common_api.cpp common_api.hpp

tcp_client.o: tcp_client.cpp tcp_client.hpp
	g++ $(flag) -c tcp_client.cpp tcp_client.hpp

tcp_server.o : tcp_server.cpp tcp_server.hpp
	g++ $(flag) -c tcp_server.cpp tcp_server.hpp

thread_pool.o: thread_pool.cpp thread_pool.hpp
	g++ $(flag) -c thread_pool.cpp thread_pool.hpp

websocket_server.o:websocket_server.cpp websocket_server.hpp
	g++ $(flag) -c websocket_server.cpp websocket_server.hpp

clean:
	-rm obj/*
	-rm *.gch