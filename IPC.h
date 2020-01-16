#pragma once

#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>

typedef struct IPCmessageToDaemon {
	char source[1024];
	char destination[1024];
	
	char task[1024];
	int jobID;
} IPCmessageToDaemon;

typedef struct IPCmessageToClient {
	int jobList[1024];
	int jobId;
	float status;
} IPCmessageToClient;


/*
Send the IPCmessageToDaemon struct to the Daemon
Use only in Clients
*/
void IPC_ClientSentMessage(IPCmessageToDaemon msg);

/*
Recive the IPCmessageToDaemon struct
returns the message
Use only in Daemon
*/
IPCmessageToDaemon IPC_DaemonReciveMessage();

/*
Send the IPCmessageToClient struct to the Client
Use only in Daemon
*/
void IPC_DaemonSentMessage(IPCmessageToClient msg);

/*
Recive the IPCmessageToClient struct
returns the message
Use only in Clients
*/
IPCmessageToClient IPC_ClientReciveMessage();


/*
Daemon Example:

#include "IPC.h"

int main() {
	IPCmessageToDaemon message;
	IPCmessageToClient sentmessage;

	while (1) {
		message = IPC_DaemonReciveMessage(); // Here the program stops and waits for a message
		printf("message recived\n");
		printf("%s\n", message.source);

		sentmessage.jobId = 5;
		IPC_DaemonSentMessage(sentmessage); // Sending message back to client
	}
	return 0;
}


Client Example:

#include "IPC.h"

int main() {
	IPCmessageToDaemon message;

	strcpy(message.source, "/c/test");
	IPC_ClientSentMessage(message);
	printf("Message sent \n");

	IPCmessageToClient reciveMessage;
	reciveMessage = IPC_ClientReciveMessage();

	printf("%d\n", reciveMessage.jobId);
	return 0;
}
*/