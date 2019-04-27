#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <time.h>
#include <assert.h>

#define  BUFFER_SIZE 1024
#define  PORT 13
#ifndef max
#define max(a, b) ( ((a) > (b)) ? (a) : (b) )
#endif


int main(int argc, char *argv[]) {
//    Server socket, Client Socket
    int server_sockfd, client_sockfd = -1, ret;

//    Length of socket len
    socklen_t sin_size;

//    Addr struct, client struct
    struct sockaddr_in server_addr, client_addr;


//    Buffer Size
    char buf[BUFFER_SIZE];

//    Reset data
    memset(&server_addr, 0, sizeof(server_addr));

//    Port, IPV4 and Any Address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

//    Create Socket
    if ((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("ERROR: Socket error");
        return 1;
    }


//    Bind socket to server
    if (bind(server_sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) < 0) {
        perror("ERROR: Bind error");
        return 1;
    }

//    Listen to request
    if (listen(server_sockfd, 5) < 0) {
        perror("ERROR: Listen error");
        return 1;
    }
    printf("Accepting connections on port %d\n", PORT);

    fd_set read_fds;
    int max_fd = server_sockfd;


    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(server_sockfd, &read_fds);
        if (client_sockfd > 0) {
            FD_SET(client_sockfd, &read_fds);
            max_fd = max(client_sockfd, max_fd);
        }

        ret = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if (ret == -1 && errno == EINTR) {
            continue;
        }

        if (ret == -1) {
            perror("ERROR: Select error");
            return 1;
        }
//        Clients incoming
        if (FD_ISSET(server_sockfd, &read_fds)) {
            sin_size = sizeof(client_addr);
//            Close if we Already have
            if (client_sockfd > 0) {
                close(client_sockfd);
            }
            if ((client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_addr, &sin_size)) < 0) {
                perror("ERROR: Accept error");
                return 1;
            }

//            Send back daytime
            printf("Accept Client %s\n", inet_ntoa(client_addr.sin_addr));
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            assert(strftime(buf, sizeof(buf), "%c", tm));
            send(client_sockfd, buf, strnlen(buf, BUFFER_SIZE), 0);
            close(client_sockfd);
            client_sockfd = -1;

        }

    }

    return 0;
}