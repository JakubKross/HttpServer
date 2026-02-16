#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

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
       if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
           perror("Accept failed");
           continue;
       }
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);
        char *querry = strstr(buffer, "GET /");
        if (strstr(buffer, "GET /status")) {
            float temp = 24.5;
            int uptime = clock() / CLOCKS_PER_SEC;

            char body[256];
            snprintf(body, sizeof(body), "Status systemu:\nTemperatura: %.2f C\n Uptime: %d sekund", temp, uptime);

            char header[512];
            snprintf(header, sizeof(header), "HTTP/1.1 200 OK\nContent-Type: text/plain; charset=utf-8\nContent-Length: %zu\n\n\n%s",strlen(body), body);
            write(new_socket, header, strlen(header));
        }
        if (querry) {
            if (strstr(buffer, "led=on")) {
                char *resp= "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nDIODA WLACZONA";
                write(new_socket, resp, strlen(resp));
            }
            else if (strstr(buffer, "led=off")) {
                char *resp = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nDIODA WYLACZONA";
            }else {
                char *html_form =
                    "HTTP/1.1 200 OK\nContent-Type: text/html\n\n"
                "<html><body>"
                "<h1>Sterowanie LED</h1>"
                "<a href=\"/?led=on\"><button>WLACZ</button></a>"
                "<a href=\"/?led=off\"><button>WYLACZ</button></a>"
                "</body></html>";
                write(new_socket, html_form, strlen(html_form));
            }
        }

        close(new_socket);
    }
    return 0;
}