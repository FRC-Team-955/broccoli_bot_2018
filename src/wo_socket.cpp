#include <wo_socket.hpp>

WriteOnlySocket::WriteOnlySocket(std::string hostname, int portno) : portno(portno), hostname(hostname) {

	server = gethostbyname(hostname.c_str());
	if (!server) throw std::runtime_error("Could not find address");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;

	bcopy((char *)server->h_addr, 
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(portno);
    keepalive();

}

size_t WriteOnlySocket::write(void* data, size_t length) {
    //printf("CALL SOCK\n");
    if (sockfd == -1) return 0;
    int ret = ::write(sockfd, data, length);
    if (ret == -1) {
        //printf("SOCK ERROR %s\n", strerror(errno));
        switch (errno) {
            case EWOULDBLOCK:
                break;
            default: disconnect(); break;
        }
        return 0;
    } else {
        return ret;
    }
}

// Returns true if the socket is connected, false otherwise. 
// Tries to connect the socket if it is not already.
bool WriteOnlySocket::keepalive() {
    // Early return if the socket is connected
    if (sockfd != -1) return true;

    // Create new socket
    sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (sockfd < 0) throw std::runtime_error("Socket could not be created.");

    // Connect new socket
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0) {
        return true;
    } else {
        printf("SOCKET ERROR: %s", strerror(errno));
        switch (errno) {
            case EINPROGRESS: //Don't spam socket recreation
                break;
            case ECONNREFUSED: //Invalidate if the connection didn't go through, try again
                disconnect();
                break;
            default: throw std::runtime_error(strerror(errno)); break;
        }
        return false;
    }
}

void WriteOnlySocket::disconnect() {
    sockfd = -1;
    close(sockfd);
}

WriteOnlySocket::~WriteOnlySocket() {
    disconnect();
}
