#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


#define PORT 7
#define SRV_IP "127.0.0.1"
#define BUFFER_SIZE 1024

//
//#ifndef strlcpy
//#define strlcpy(dst, src, sz) snprintf((dst), (sz), "%s", (src))
//#endif

void handle_error(const char *msg) {
    fprintf(stderr, "ERROR: %s", msg);
}


char *inputString(FILE *fp, size_t *size) {
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char) * (*size));//size is start size
    if (!str)
        return str;
    while (EOF != (ch = fgetc(fp)) && ch != '\n') {
        str[len++] = ch;
        if (len == *size) {
            str = realloc(str, sizeof(char) * (*size += 16));
            if (!str)
                return str;
        }
    }
    str[len++] = '\n';
    str = realloc(str, sizeof(char) * (*size += 1));
    str[len++] = '\0';
    *size = len;
    return realloc(str, sizeof(char) * len);
}


int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    char buf[BUFFER_SIZE];
    int client_sockfd;

//    Create socket
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sockfd == -1) {
        handle_error("socket()");
        exit(1);
    }

//    Set server info
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SRV_IP, &server_addr.sin_addr) == 0) {
        handle_error("inet_pton()");
        exit(1);
    }

//    Connect to server
    if ((connect(client_sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)) {
        handle_error("Connect()");
        exit(1);
    }


    printf("Connect to server ip:%s port:%d\n", SRV_IP, PORT);


    while (1) {
        size_t len = 0;
        char *msg = inputString(stdin, &len);
        len--;
        size_t chunks = (len / (BUFFER_SIZE - 1)) + 1;

        for (int i = 0; i < chunks; ++i) {
            strncpy(buf, msg + (i * (BUFFER_SIZE - 1)), BUFFER_SIZE - 1);
            if ((len = send(client_sockfd, buf, strnlen(buf, BUFFER_SIZE), 0)) < 0) {
                free(msg);
                msg = NULL;
                handle_error("Send()");
                exit(1);
            }

            if ((len = recv(client_sockfd, buf, BUFFER_SIZE, 0)) < 0) {
                handle_error("Recv()");
                free(msg);
                msg = NULL;
                exit(1);
            }

            buf[len] = '\0';
            printf("%s", buf);
            fflush(stdout);

        }
        free(msg);
        msg = NULL;
    }

    return 0;
}
