#include <stdio.h>
#include <stdlib.h>
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
    time_t start_time = time(NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family =AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    printf("Serwer Urchomiony na porcie %d\n", PORT);

    while(1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) continue;

        char buffer[1024] = {0};
        read (new_socket, buffer, 1024);

        if(strstr(buffer, "GET /status")){
            float temp = 24.5 + (rand() % 10) / 10.0;
            time_t current_time = time(NULL);

            int uptime = (int)(current_time - start_time);

            char body[256];
            snprintf(body, sizeof(body), "Status systemu:\nTemperatura: %.2f C\nUptime: %d sekund", temp, uptime);

            char header[512];
            snprintf(header, sizeof(header), "HTTP/1.1 200 OK\nContent-Type: text/plain; charset=utf-8\nContent-Length: %zu\n\n%s", strlen(body), body);
            write(new_socket, header, strlen(header));
        } else if(strstr(buffer, "led_on")){
            char *resp = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nDIODA WYLACZONA";
            write(new_socket, resp, strlen(resp));
        }
        else if (strstr(buffer, "GET / ")){
            char *html_form = 
            "HTTP/1.1 200 OK\nContent-Type: text/html\n\n"
            "<html><body>"
            "<h1>Sterowanie LED</h1>"
            "<a href=\"/?led=on\"><button>WLACZ</button></a>"
            "<a> href=\"/?led=off\"><button>WYLACZ</button></a>"
            "<br><br><a hre\"/status\">Sprawdz status</a>"
            "</body></html>";
            write(new_socket, html_form, strlen(html_form));
        }
        else{
            char *not_found = "HTTP/1.1 404 Not Found\nContent=Length: 18\n\nStrona nie istnieje";
            write(new_socket, not_found, strlen(not_found));
        }
        close(new_socket);
    }
    return 0;
}