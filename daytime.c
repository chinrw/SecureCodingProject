#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


#define PORT 13
#define SRV_IP "127.0.0.1"
#define BUFFER_SIZE 1024

void handle_error(const char *msg) {
    fprintf(stderr, "ERROR: %s", msg);
}


int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    char buf[BUFFER_SIZE];
    int client_sockfd;


//    Check we have the right number of arguments
    if (argc != 3) {
        printf("usage: %s <IP Address> <Port>\n", argv[0]);
        exit(1);
    }


//    Create socket
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sockfd == -1) {
        handle_error("socket()");
        exit(1);
    }

//    Set server info
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(strtol(argv[2], &argv[2], 10));

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) == 0) {
        handle_error("inet_pton()");
        exit(1);
    }

//    Connect to server
    if ((connect(client_sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)) {
        handle_error("Connect()");
        exit(1);
    }

    socklen_t len;
    printf("Connect to server ip:%s port:%s\n", argv[1], argv[2]);
    if ((len = recv(client_sockfd, buf, BUFFER_SIZE, 0)) < 0) {
        handle_error("Recv()");
        exit(1);
    }
    buf[len] = '\0';
    printf("%s", buf);
    fflush(stdout);

    return 0;
}
