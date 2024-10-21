#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DEST_IP "192.168.1.2"  // Replace with the actual destination IP
#define BUFF_SIZE 65536

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int main() {
    int sockfd;
    char buffer[BUFF_SIZE];

    struct sockaddr_in dest_addr;

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(0);
    inet_pton(AF_INET, DEST_IP, &dest_addr.sin_addr);

    struct iphdr *ip_header = (struct iphdr *)buffer;
    memset(buffer, 0, BUFF_SIZE);

    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = sizeof(struct iphdr);
    ip_header->id = htonl(54321);
    ip_header->frag_off = 0;
    ip_header->ttl = 255;
    ip_header->protocol = IPPROTO_TCP;
    ip_header->check = 0;
    ip_header->saddr = inet_addr("192.168.1.1");  // Replace with your source IP
    ip_header->daddr = inet_addr(DEST_IP);

    ip_header->check = checksum((unsigned short *)buffer, ip_header->tot_len);

    if (sendto(sockfd, buffer, ip_header->tot_len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("Packet sending failed");
        return 1;
    }

    printf("Packet sent to %s\n", DEST_IP);

    close(sockfd);
    return 0;
}
