// file name: UdpServer.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFLEN 512

void error(char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    int server_fd;
    int recv_len;
    char buf[BUFLEN] = {0};
    struct sockaddr_in si_me, si_other;
    int slen = sizeof(si_other);

    if (argc < 2) {
	fprintf(stderr, "usage: %s port\n", argv[0]);
	exit(1);
    }

    // create socket
    if	((server_fd = socket(AF_INET, SOCK_DGRAM, 0))==-1) {
	error("Error to create socket.");
    }

    // bind
    memset(&si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(atoi(argv[1]));
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_fd, (const struct sockaddr *)&si_me, sizeof(si_me)) == -1) {
	error("Error when bind to port");
    }

    // receive and answer
    while (1) {
	memset(buf, 0, BUFLEN);
	printf("Waiting from data ...\n");
	if ((recv_len = recvfrom(server_fd, buf, BUFLEN-1, 0, (struct sockaddr *)&si_other, &slen)) == -1) {
	    error("Error receive from udp client.");
	}
	printf("Receive : %s\n", buf);
	printf("Answer : %s\n", buf);
	if (sendto(server_fd, buf, strlen(buf), 0, (const struct sockaddr *)&si_other, slen) == -1) {
	    error("Error send to udp client.");
	}
	if (strncmp(buf, "kill", 4) == 0) {
	    break;
	}
    }
    close(server_fd);
    return 0;
}

