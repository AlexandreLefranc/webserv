#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5000
#define MAX_MESSAGE_LEN 256

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in server_addr;
    char message[MAX_MESSAGE_LEN];
    int recv_len;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(1);
    }
    printf("Socket created.\n");

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(1);
    }
    printf("binding socket to port 5000.\n");

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("listen failed");
        exit(1);
    }

    // Accept incoming connection
    int client_sockfd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    client_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_addr_len);
    if (client_sockfd < 0) {
        perror("accept failed");
        exit(1);
    }
    printf("Accepted incoming connection.\n");

    // Receive message from client
    memset(message, 0, MAX_MESSAGE_LEN);
    recv_len = recv(client_sockfd, message, MAX_MESSAGE_LEN - 1, 0);
    if (recv_len < 0) {
        perror("recv failed");
        exit(1);
    }

    // Print received message
    printf("Received message: %s\n", message);
    send(client_sockfd, "Got it thx.\n", 12, 0);

    // Close socket
    close(sockfd);
    close(client_sockfd);

    return 0;
}

