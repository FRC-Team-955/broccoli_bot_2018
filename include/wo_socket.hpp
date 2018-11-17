#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h> 
#include <string>
#include <errno.h>
#include <unistd.h>
#include <iostream>

class WriteOnlySocket {
    public:
        WriteOnlySocket (std::string hostname, int portno);
        size_t write(void* data, size_t length);
        ~WriteOnlySocket ();
        bool keepalive();
    private:
        void disconnect();
		int sockfd, portno;
        bool socket_is_connected = false;
		struct sockaddr_in serv_addr;
		struct hostent *server;
        std::string hostname;
};
