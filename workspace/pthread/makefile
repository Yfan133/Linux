all:pthexit thread test
pthexit:pthexit.c
	g++ $^ -o $@ -lpthread
thread:thread.c
	g++ $^ -o $@ -lpthread
test:test.c
	gcc $^ -o $@ -lpthread
