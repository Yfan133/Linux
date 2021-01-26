INCLUDE_PATH=-I /home/test/third_part/include -I /usr/include/jsoncpp
LIB_PATH=-L /home/test/third_part/lib -lpthread -lctemplate -ljsoncpp

BIN=../bin/svr

$(BIN):oj_server.cpp
	g++ $^ -o $@ -g $(INCLUDE_PATH) $(LIB_PATH)

.PHONY:clean
clean:
	rm $(BIN)
