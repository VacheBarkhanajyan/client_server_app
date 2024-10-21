#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFF_SIZE 65536

int main() {
    int sockfd;
    struct sockaddr_in client_addr;
    char buffer[BUFF_SIZE];
    socklen_t addr_len;

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    printf("Server is running and waiting for incoming packets...\n");

    while (1) {
        addr_len = sizeof(client_addr);
        int data_len = recvfrom(sockfd, buffer, BUFF_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (data_len < 0) {
            perror("Error receiving data");
            return 1;
        }

        printf("Received %d bytes from %s\n", data_len, inet_ntoa(client_addr.sin_addr));
    }

    close(sockfd);
    return 0;
}
