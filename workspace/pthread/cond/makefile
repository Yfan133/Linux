all:rlock noodles consumers
noodles:noodles.c
	g++ $^ -o $@ -lpthread
rlock:rlock.c
	g++ $^ -o $@ -lpthread -g
consumers:consumers.c
	g++ $^ -o $@ -lpthread 
