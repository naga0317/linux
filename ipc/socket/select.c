#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) 
{
    int server_fd, client_fds[MAX_CLIENTS], max_fd, activity, i, valread;
    struct sockaddr_in address;
    fd_set read_fds;
    int port = 8080;
    char buffer[BUFFER_SIZE];

    memset(client_fds, 0, sizeof(client_fds));

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0) 
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    while(1) 
    {
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        max_fd = server_fd;

        for(i = 0; i < MAX_CLIENTS; i++) 
	{
            int fd = client_fds[i];
            if (fd > 0) 
	    {
                FD_SET(fd, &read_fds);
            }
            if (fd > max_fd) 
	    {
                max_fd = fd;
            }
        }

        activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if(activity < 0) 
	{
            perror("select error");
            exit(EXIT_FAILURE);
        }

        if(FD_ISSET(server_fd, &read_fds)) 
	{
            int new_fd;
            struct sockaddr_in client_address;
            int addrlen = sizeof(client_address);

            if((new_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*)&addrlen)) < 0) 
	    {
                perror("accept error");
                exit(EXIT_FAILURE);
            }

            printf("New client connected from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

            for(i = 0; i < MAX_CLIENTS; i++) 
	    {
                if (client_fds[i] == 0) 
		{
                    client_fds[i] = new_fd;
                    break;
                }
            }
        }
        for(i = 0; i < MAX_CLIENTS; i++) 
	{
            int fd = client_fds[i];
            if(FD_ISSET(fd, &read_fds)) 
	    {
                valread = read(fd, buffer, BUFFER_SIZE);
                if (valread == 0) 
		{
                    printf("Client disconnected\n");
                    close(fd);
                    client_fds[i] = 0;
                } 
		else 
		{
                    buffer[valread] = '\0';
                    printf("Received message from client %d: %s", fd, buffer);
                    write(fd, buffer, strlen(buffer));
                }
            }
        }
    }
    return 0;
}

