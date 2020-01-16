#include "IPC.h"
#include "threadService.h"



int main() {
    maxThreads = 2;
    IPCmessageToDaemon client_message;
    IPCmessageToClient sentmessage;

    while (1) {
        client_message = IPC_DaemonReciveMessage();
        printf("%s\n", client_message.task);
        if (strcmp(client_message.task, "copy") == 0)
        {
            int id = copyThread(&client_message);
            sentmessage.jobId = id;
            IPC_DaemonSentMessage(sentmessage);
        }
        else if (strcmp(client_message.task, "status") == 0)
        {
            printf("STATUS");
            float status = statusThread(client_message.jobID);
            sentmessage.status = status;
            IPC_DaemonSentMessage(sentmessage);
        }
        else if (strcmp(client_message.task, "stop") == 0)
        {
            stopThread(client_message.jobID);
        }
        else if (strcmp(client_message.task, "suspend") == 0)
        {
            pauseThread(client_message.jobID);
        }
        else if (strcmp(client_message.task, "resume") == 0)
        {
            resumeThread(client_message.jobID);
        }
        /*printf("%s\n",client_message.task);
        printf("%s\n", client_message.source);
        printf("%s\n", client_message.destination);
        printf("%d\n", client_message.jobID);*/

    }
    return 0;
}


