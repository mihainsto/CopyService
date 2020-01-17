#include "threadService.h"
#include "IPC.h"
int copy(void* arg) {
    IPCmessageToDaemon* data = arg;
    char v[2][1024];
    strcpy(v[0], data->source);
    strcpy(v[1], data->destination);
    FILE* source = open(v[0], O_RDONLY);

    if (source == -1) {
        printf("Eroare la deschiderea fisierului sursa!\n");
        return NULL;
    }
    FILE* destination = open(v[1], O_WRONLY | O_CREAT);
    if (destination == -1) {
        printf("Eroare la deschiderea fisierului destinatie!\n");
        return NULL;
    }
    char* buffer;
    buffer = malloc(1024);
    int chunks = 0;
    size_t were_read = read(source, buffer, 1024);
    while (were_read == 1024) {
        size_t dest = write(destination, buffer, 1024);
        if (dest == -1) {
            printf("Eroare la scrierea in fisier!");
            return -1;
        }

        chunks++;
        struct stat sb;
        float p = (float)(chunks * 1024) / (long long)sb.st_size;
        if (p > 1)
            p = 1;
        pthread_mutex_lock(&progressMutex);
        progress[data->jobID] = p;
        pthread_mutex_unlock(&progressMutex);

        were_read = read(source, buffer, 1024);

        while (1)
        {
            pthread_mutex_lock(&pauseMutex);
            if (pause_status[data->jobID] == 0)  //Nu a fost oprit 
            {
                pthread_mutex_unlock(&pauseMutex);
                break;
            }
            pthread_mutex_unlock(&pauseMutex);
            sleep(1);
        }
    }
    if (were_read == -1)
    {
        printf("Eroare la citire din fisier");
        return -1;
    }
    else
    {
        size_t dest = write(destination, buffer, were_read);
        if (dest == -1)
        {
            printf("Eroare la scriere");
            return -1;
        }

    }
    pthread_mutex_lock(&mtx);
    threads[data->jobID] = 0;
    pthread_mutex_unlock(&mtx);
}
void stopThread(int index)
{
    pthread_mutex_lock(&mtx);
    pthread_cancel(threads[index]);
    threads[index] = 0;
    pthread_mutex_unlock(&mtx);
}
// start a thread 
int copyThread(IPCmessageToDaemon* client_message)
{
    pthread_mutex_lock(&copyThreadMutex);
    pthread_mutex_lock(&mtx);

    int i;
    for (i = 0; i < maxThreads; i++) // search for a place for the thread 
    {
        if (threads[i] == 0) {
            break;
        }
    }
    pthread_mutex_unlock(&mtx);

    if (i == maxThreads) // if we didn't found any place
    {
        while (1)
        {
            sleep(1);
            pthread_mutex_lock(&mtx);
            for (i = 0; i < maxThreads; i++) // search for a place for the thread 
            {
                if (threads[i] == 0) {
                    break;
                }
            }
            if (i != maxThreads) // we found a place
            {
                client_message->jobID = i;
                pthread_create(&threads[i], NULL, copy, client_message);
                pthread_mutex_unlock(&mtx);
                pthread_mutex_unlock(&copyThreadMutex);
                return i;
            }

            pthread_mutex_unlock(&mtx);

        }
    }
    else // we found a place in the first scan
    {

        pthread_mutex_lock(&mtx);
        client_message->jobID = i;
        pthread_create(&threads[i], NULL, copy, client_message);
        pthread_mutex_unlock(&mtx);
        pthread_mutex_unlock(&copyThreadMutex);
        return i;
    }
}

float statusThread(int jobID)
{
    pthread_mutex_lock(&progressMutex);
    float val = progress[jobID];
    pthread_mutex_unlock(&progressMutex);
    return val;
}
// PAUSE THREAD
void pauseThread(int jobID)
{
    pthread_mutex_lock(&pauseMutex);
    pause_status[jobID] = 1;
    pthread_mutex_unlock(&pauseMutex);
}

void resumeThread(int jobID)
{
    pthread_mutex_lock(&pauseMutex);
    pause_status[jobID] = 0;
    pthread_mutex_unlock(&pauseMutex);
}
int config(char* filename)
{
    int a;
    FILE* f = fopen(filename, "r");
    if (f == NULL)
        printf("Eroare la deschiderea fisierului");

    fscanf(f, "%d", &a);
    fclose(f);

    return a;
}
