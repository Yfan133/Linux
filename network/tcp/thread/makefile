all : svr cli
svr : svr_thread.cpp
	g++ $^ -o $@ -g -lpthread
cli : cli_thread.cpp
	g++ $^ -o $@ -g
