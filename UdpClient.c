// file name: UdpClient.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFLEN 512

void error( char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    int clientSocket, portNum, nBytes;
	int sendSize, recvSize;
    char buffer[BUFLEN];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    if (argc < 3) {
	fprintf(stderr, "usage: %s ip port.\n", argv[0]);
	exit(1);
    }

    printf("connect to %s : %s.\n", argv[1], argv[2]);

    // create socket
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
	error("error to create udp socket.");
    }

    // send and answer
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
    addr_size = sizeof(serverAddr);
    while (1) {
	printf("Enter data to send: \n");
	fgets(buffer, BUFLEN, stdin);
	nBytes = strlen(buffer) + 1;
	
	sendSize = sendto(clientSocket, buffer, nBytes, 0, (const struct sockaddr *)&serverAddr, addr_size);
	printf("Send: %s, size: %d\n", buffer, sendSize);
	if (sendSize == -1) {
		printf("error number: %d, msg: %s\n", errno, strerror(errno));
	}
	
	memset(buffer, '\0', sizeof(buffer));
	recvSize = recvfrom(clientSocket, buffer, BUFLEN, 0, NULL, NULL);
	printf("Answer: %s, size: %d\n", buffer, recvSize);
	if (recvSize == -1) {
		printf("error number: %d, msg: %s\n", errno, strerror(errno));
	}
    }
    return 0;
}
