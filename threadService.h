#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include "IPC.h"

pthread_t threads[1024];
int maxThreads;
int pause_status[1024];
float progress[1024];
pthread_mutex_t mtx;
pthread_mutex_t copyThreadMutex;
pthread_mutex_t pauseMutex;
pthread_mutex_t pauseMutex;
pthread_mutex_t progressMutex;


void stopThread(int index);
int copyThread(IPCmessageToDaemon* client_message);
void pauseThread(int jobID);
float statusThread(int jobID);
void resumeThread(int jobID);