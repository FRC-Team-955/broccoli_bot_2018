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

/* Simple write-conly socket client. Tolerant to connection hangups 
 * TODO: Don't spam connect() and socket()
 */
class WriteOnlySocket {
    public:
        WriteOnlySocket (std::string hostname, int portno);
        size_t write(void* data, size_t length);

        /* Call this to check connection and attempt reconnection if possible. Returns false
         * when disconnected
         */
        bool keepalive();

        ~WriteOnlySocket ();
    private:
        void disconnect();
		int sockfd = -1;
        int portno = 0;
		struct sockaddr_in serv_addr;
		struct hostent *server;
        std::string hostname;
};
