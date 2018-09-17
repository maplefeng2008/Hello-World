// file name: TcpServer.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    int sockfd;
    int newsockfd;
    int portno;
    int clilen;
    int n;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    
    if (argc < 2) {
	error("Error: no port number provided.");
    }
    portno = atoi(argv[1]);
    
    // create socket and bind
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
	error("Error: failed to open socket.");
    }    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (const struct sockaddr*)&serv_addr,
	sizeof(serv_addr)) < 0) {
	error("Error: binding.");
    }
    
    // listen and accept
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    printf("Wait for connect.\n");
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    if (newsockfd < 0) {
	error("Error on accept.");
    }
    
    // read and write
    memset(buffer, 0, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0) {
	error("Error reading from socket.");
    }
    printf("Recieve: %s\n", buffer);
    printf("Reply: I got your message.\n");
    n = write(newsockfd, "I got your message.", 18);
    if (n < 0) {
	error("Error writing to socket.");
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}
