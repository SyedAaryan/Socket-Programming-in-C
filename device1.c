#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Error function to handle errors
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    // Check if port number is provided
    if (argc < 2) {
        fprintf(stderr, "Port number not provided. Program terminated.\n");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }

    // Zero out the server address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]); // Convert port number from string to integer

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the socket to the port
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("Binding failed");
    }

    // Listen for incoming connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Accept incoming connection
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        error("Error on accept");
    }

    // Communication loop
    while (1) {
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);
        if (n < 0) {
            error("Error reading from socket");
        }
        printf("Client: %s\n", buffer);
        bzero(buffer, 255);

        if (fgets(buffer, 255, stdin) == NULL) {
            error("Error reading from stdin");
        }

        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0) {
            error("Error writing to socket");
        }

        // Exit loop if the client sends "Bye"
        if (strncmp("Bye", buffer, 3) == 0) {
            break;
        }
    }

    // Close sockets
    close(newsockfd);
    close(sockfd);
    return 0;
}
