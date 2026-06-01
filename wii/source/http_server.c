#include "http_server.h"

#include "ai_brain.h"
#include "chat_page.h"

#include <network.h>
#include <ogc/lwp_watchdog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#define REQUEST_BUF_SIZE 2048
#define PATH_BUF_SIZE 512
#define MSG_BUF_SIZE 512
#define JSON_BUF_SIZE 1024

static int hex_to_int(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1;
}

static void url_decode(const char* in, char* out, size_t out_size)
{
    size_t i = 0;
    size_t j = 0;

    if (!out_size) return;

    while (in[i] != '\0' && j + 1 < out_size) {
        if (in[i] == '+') {
            out[j++] = ' ';
            ++i;
        } else if (in[i] == '%' && in[i + 1] && in[i + 2]) {
            int hi = hex_to_int(in[i + 1]);
            int lo = hex_to_int(in[i + 2]);
            if (hi >= 0 && lo >= 0) {
                out[j++] = (char)((hi << 4) | lo);
                i += 3;
            } else {
                out[j++] = in[i++];
            }
        } else {
            out[j++] = in[i++];
        }
    }
    out[j] = '\0';
}

static void json_escape(const char* in, char* out, size_t out_size)
{
    size_t i;
    size_t j = 0;

    if (!out_size) return;

    for (i = 0; in[i] != '\0' && j + 1 < out_size; ++i) {
        char c = in[i];
        if ((c == '\\' || c == '"') && j + 2 < out_size) {
            out[j++] = '\\';
            out[j++] = c;
        } else if (c == '\n' && j + 2 < out_size) {
            out[j++] = '\\';
            out[j++] = 'n';
        } else {
            out[j++] = c;
        }
    }
    out[j] = '\0';
}

static void send_response(int client, const char* status, const char* content_type, const char* body)
{
    char header[384];
    size_t body_len = strlen(body);

    snprintf(header, sizeof(header),
             "HTTP/1.0 %s\r\n"
             "Content-Type: %s\r\n"
             "Access-Control-Allow-Origin: *\r\n"
             "Connection: close\r\n"
             "Content-Length: %u\r\n\r\n",
             status, content_type, (unsigned)body_len);

    net_send(client, header, strlen(header), 0);
    net_send(client, body, body_len, 0);
}

static void handle_chat(int client, const char* query)
{
    const char* msg_key = strstr(query, "msg=");
    char raw[MSG_BUF_SIZE];
    char decoded[MSG_BUF_SIZE];
    char escaped[JSON_BUF_SIZE];
    char body[JSON_BUF_SIZE + 64];
    const char* response;

    if (!msg_key) {
        send_response(client, "400 Bad Request", "application/json", "{\"response\":\"Missing msg parameter\"}");
        return;
    }

    msg_key += 4;
    {
        size_t i = 0;
        while (msg_key[i] != '\0' && msg_key[i] != '&' && i < sizeof(raw) - 1) {
            raw[i] = msg_key[i];
            ++i;
        }
        raw[i] = '\0';
    }

    url_decode(raw, decoded, sizeof(decoded));
    response = ai_brain_respond(decoded);
    json_escape(response, escaped, sizeof(escaped));
    snprintf(body, sizeof(body), "{\"response\":\"%s\"}", escaped);
    send_response(client, "200 OK", "application/json", body);
}

static void handle_request(int client, const char* request)
{
    char method[8];
    char path[PATH_BUF_SIZE];

    if (sscanf(request, "%7s %511s", method, path) != 2) {
        send_response(client, "400 Bad Request", "text/plain", "Bad Request");
        return;
    }

    if (strcmp(method, "GET") != 0) {
        send_response(client, "405 Method Not Allowed", "text/plain", "Only GET supported");
        return;
    }

    if (strcmp(path, "/") == 0) {
        send_response(client, "200 OK", "text/html; charset=utf-8", CHAT_PAGE_HTML);
        return;
    }

    if (strncmp(path, "/chat", 5) == 0 && (path[5] == '\0' || path[5] == '?')) {
        const char* query = strchr(path, '?');
        if (!query || query[1] == '\0') {
            send_response(client, "400 Bad Request", "application/json", "{\"response\":\"Missing query\"}");
            return;
        }
        handle_chat(client, query + 1);
        return;
    }

    send_response(client, "404 Not Found", "text/plain", "Not Found");
}

int http_server_run(unsigned short port, const char* ip_address, int (*should_exit)(void))
{
    int server_fd;
    struct sockaddr_in server_addr;

    printf("HTTP server listening on http://%s:%u/\n", ip_address ? ip_address : "0.0.0.0", port);
    printf("Waiting for iPad/browser connections...\n");

    server_fd = net_socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (server_fd < 0) {
        printf("net_socket failed: %d\n", server_fd);
        return server_fd;
    }

    {
        int yes = 1;
        net_setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (net_bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("net_bind failed. Port 80 may require elevated IOS privileges.\n");
        net_close(server_fd);
        return -1;
    }

    if (net_listen(server_fd, 4) < 0) {
        printf("net_listen failed\n");
        net_close(server_fd);
        return -2;
    }

    while (!should_exit || !should_exit()) {
        fd_set fds;
        struct timeval tv;
        int ready;

        FD_ZERO(&fds);
        FD_SET(server_fd, &fds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        ready = net_select(server_fd + 1, &fds, NULL, NULL, &tv);
        if (ready < 0) {
            printf("net_select error: %d\n", ready);
            break;
        }

        if (ready == 0) {
            continue;
        }

        {
            int client;
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            char request[REQUEST_BUF_SIZE];
            int recv_len;

            client = net_accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
            if (client < 0) {
                continue;
            }

            recv_len = net_recv(client, request, sizeof(request) - 1, 0);
            if (recv_len > 0) {
                request[recv_len] = '\0';
                handle_request(client, request);
            } else {
                send_response(client, "400 Bad Request", "text/plain", "Empty request");
            }

            net_close(client);
        }
    }

    net_close(server_fd);
    printf("HTTP server stopped.\n");
    return 0;
}
