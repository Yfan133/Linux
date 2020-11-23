#include<stdio.h>
#include<unistd.h>
#include<queue>
#include<pthread.h>
#include<iostream>

#define THREADCOUNT 4

typedef (void*) (*Handler_t)(int);  //
