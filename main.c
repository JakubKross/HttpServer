#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<";


    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_fd, 3)< 0) {
        perror("Listen failed");
        return 1;

    }
    printf("Serwer Uruchomiony");

    while (1) {
        printf("\n Czekam na połączenie...http://%s:%d\n", "192.168.0.86", PORT);
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);


        if (new_socket <0) {
            continue;
        }
        printf("Klient Połączony\n");
        char buffer[2048] = {0};
        long valread = read(new_socket, buffer, 2048);
        if (valread > 0) {
            printf("Otrzymano dane od przeglądarki:\n%s\n", buffer);
        }

        char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n"
        "Content-Length: 13\r\n"
        "Connection: Close\r\n"
        "\r\n"
        "Hello World!\n";

        send(new_socket, response, strlen(response), 0);
        printf("Odpowiedź wysłana.\n");

        shutdown(new_socket, SHUT_RDWR);
        close(new_socket);
    }
    return 0;
}