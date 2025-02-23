#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <sqlite3.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "dbmanager.h"

#define PORT 8080
#define FRONT_DIR "../Front"

sqlite3 *db;

char* read_file(const char* path, size_t* length) 
{
    FILE* file = fopen(path, "rb");
    if (!file) return NULL;

    struct stat st;
    stat(path, &st);
    *length = st.st_size;

    char* content = malloc(*length + 1);
    fread(content, 1, *length, file);
    content[*length] = '\0';
    fclose(file);
    return content;
}

void handle_request(int client_fd) 
{
    char buffer[4096];
    read(client_fd, buffer, sizeof(buffer));

    if (strstr(buffer, "GET /index") || strstr(buffer, "GET / ")) 
    {
        size_t len;
        char* content = read_file(FRONT_DIR "/index.html", &len);
        if (!content) 
        {
            char* resp = "HTTP/1.1 404 Not Found\r\n\r\n";
            send(client_fd, resp, strlen(resp), 0);
            return;
        }

        char header[512];
        snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n\r\n",
            len);
        
        send(client_fd, header, strlen(header), 0);
        send(client_fd, content, len, 0);
        free(content);
    }
    else if (strstr(buffer, "GET /add")) 
    {
        size_t len;
        char* content = read_file(FRONT_DIR "/add.html", &len);
        if (!content) 
        {
            char* resp = "HTTP/1.1 404 Not Found\r\n\r\n";
            send(client_fd, resp, strlen(resp), 0);
            return;
        }

        char header[512];
        snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n\r\n",
            len);
        
        send(client_fd, header, strlen(header), 0);
        send(client_fd, content, len, 0);
        free(content);
    }
    else if (strstr(buffer, "GET /quotes")) 
    {
        size_t len;
        char* content = read_file(FRONT_DIR "/quotes.html", &len);
        if (!content) 
        {
            char* resp = "HTTP/1.1 404 Not Found\r\n\r\n";
            send(client_fd, resp, strlen(resp), 0);
            return;
        }

        char header[512];
        snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n\r\n",
            len);
        
        send(client_fd, header, strlen(header), 0);
        send(client_fd, content, len, 0);
        free(content);
    }
    else if (strstr(buffer, "GET /getquotes")) 
    {
        size_t len;
        Qlist Quotes;
        GetQuotes(&Quotes, db);
        printf("%d\n", Quotes.ElementsNumber);
        char* content = malloc((2000 * Quotes.ElementsNumber + 2) * sizeof(char));
        
        strcpy(content, "[");
        EQlist *P = Quotes.head;

        if (P != NULL)
        {
            while (P->next != NULL)
            {
                cJSON *json = cJSON_CreateObject(); 
                cJSON_AddStringToObject(json, "name", P->quote.Name); 
                cJSON_AddStringToObject(json, "country", P->quote.Country); 
                cJSON_AddStringToObject(json, "quote", P->quote.QuoteBody); 
                strcat(content, cJSON_Print(json));
                cJSON_Delete(json); 
                strcat(content, ",\n");
                P = P->next;
            }

            cJSON *json = cJSON_CreateObject(); 
            cJSON_AddStringToObject(json, "name", P->quote.Name); 
            cJSON_AddStringToObject(json, "country", P->quote.Country); 
            cJSON_AddStringToObject(json, "quote", P->quote.QuoteBody); 
            strcat(content, cJSON_Print(json));
            cJSON_Delete(json); 

            strcat(content, "\n");
        }

        strcat(content, "]");

        printf("%s\n", content);

        len = strlen(content);

        char header[512];
        snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n\r\n"
            ,
            len);
        
        send(client_fd, header, strlen(header), 0);
        send(client_fd, content, len, 0);
        
        while (Quotes.head != NULL)
        {
            P = Quotes.head;
            Quotes.head = Quotes.head->next;
            free(P);
            Quotes.ElementsNumber -= 1;
        }
        
    }
    else if (strstr(buffer, "POST / ")) 
    {
        char* body = strstr(buffer, "\r\n\r\n");
        if (body) 
        {
            body += 4;
            printf("Received data: %s\n", body);

            char *pointer;
            char *end;
            Quote quote;
            strcpy(quote.Name, "");
            strcpy(quote.Country, "");
            strcpy(quote.QuoteBody, "");

            cJSON *root = cJSON_Parse(body);
            if (root == NULL) {
                printf("Error parsing JSON!\n");
                return;
            }

            cJSON *name = cJSON_GetObjectItem(root, "name");
            cJSON *country = cJSON_GetObjectItem(root, "country");
            cJSON *quot = cJSON_GetObjectItem(root, "quote");

            strcpy(quote.Name, name->valuestring);
            strcpy(quote.Country, country->valuestring);
            strcpy(quote.QuoteBody, quot->valuestring);

            AddQuote(db, quote);

            char* response = "HTTP/1.1 200 OK\r\n"
                             "Content-Type: application/json\r\n"
                             "Content-Length: 29\r\n\r\n"
                             "{\"status\":\"success\",\"data\":1}";
            send(client_fd, response, strlen(response), 0);
        }
    }
    else 
    {
        const char* path = FRONT_DIR;
        if (strstr(buffer, "GET /style.css")) {path = FRONT_DIR "/style.css";}
        else if (strstr(buffer, "GET /script.js")) {path = FRONT_DIR "/script.js";}
        else {path = FRONT_DIR "/notfound.html";}

        size_t len;
        char* content = read_file(path, &len);
        if (content) 
        {
            const char* mime = "text/plain";
            if (strstr(path, ".css")) {mime = "text/css";}
            else if (strstr(path, ".js")) {mime = "application/javascript";}
            else {mime = "text/html";}

            char header[512];
            snprintf(header, sizeof(header),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: %s\r\n"
                "Content-Length: %zu\r\n\r\n",
                mime, len);
            
            send(client_fd, header, strlen(header), 0);
            send(client_fd, content, len, 0);
            free(content);
        }
    }
}

int main() 
{
    if (sqlite3_open("quotes.db", &db))
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return EXIT_FAILURE;
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, SOMAXCONN);

    printf("Server running on http://localhost:%d\n", PORT);

    while(1) 
    {
        int client_fd = accept(server_fd, NULL, NULL);
        handle_request(client_fd);
        close(client_fd);
    }

    return 0;
}