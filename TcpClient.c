// file name: TcpClient.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error( char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[256] = {0};

    if (argc < 3) {
	fprintf(stderr, "usage: %s hostname port\n", argv[0]);
	exit(1);
    }
    portno = atoi(argv[2]);

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
	error("Error opening socket.");
    }

    // connet
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
	error("Error connecting.");
    }
    
    // write and read
    printf("Please enter the massage: ");
    fgets(buffer, 255, stdin);
    printf("Send: %s\n", buffer);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
	error("Error writing to socket.");
    }
    memset(buffer, 0, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) {
	error("Error reading from socket.");
    }
    printf("Answer: %s\n", buffer);
    return 0;
}
