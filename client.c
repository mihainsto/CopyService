#include "IPC.h"

int main(int argc, char* argv[])
{
    char* job;
    job = malloc(strlen(argv[1]));
    strcpy(job, argv[1]);
    IPCmessageToDaemon client_message;
    strcpy(client_message.task, job);

    if (strcmp(client_message.task, "copy") == 0)
    {
        char* source = malloc(strlen(argv[2]));
        strcpy(source, argv[2]);
        char* destination = malloc(strlen(argv[3]));
        strcpy(destination, argv[3]);

        strcpy(client_message.source, source);
        strcpy(client_message.destination, destination);
        IPC_ClientSentMessage(client_message);
        IPCmessageToClient response = IPC_ClientReciveMessage();
        printf("%d\n", response.jobId);
        printf("sent message");
    }
    else if (strcmp(client_message.task, "status") == 0)
    {
        int ID = atoi(argv[2]);
        client_message.jobID = ID;
        IPC_ClientSentMessage(client_message);
        IPCmessageToClient response = IPC_ClientReciveMessage();
        printf("%d\n", response.status);
    }
    else if (strcmp(client_message.task, "stop") == 0)
    {
        int ID = atoi(argv[2]);
        client_message.jobID = ID;
        IPC_ClientSentMessage(client_message);
    }
    else if (strcmp(client_message.task, "suspend") == 0)
    {
        int ID = atoi(argv[2]);
        client_message.jobID = ID;
        IPC_ClientSentMessage(client_message);
    }
    else if (strcmp(client_message.task, "resume") == 0)
    {
        int ID = atoi(argv[2]);
        client_message.jobID = ID;
        IPC_ClientSentMessage(client_message);
    }
    return 0;
}