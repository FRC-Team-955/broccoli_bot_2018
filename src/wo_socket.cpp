#include <wo_socket.hpp>

WriteOnlySocket::WriteOnlySocket(std::string hostname, int portno) : portno(portno), hostname(hostname) {
	sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (sockfd < 0) throw std::runtime_error("Socket could not be created.");

	server = gethostbyname(hostname.c_str());
	if (!server) throw std::runtime_error("Could not find address");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;

	bcopy((char *)server->h_addr, 
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(portno);
    try_connect();
}

size_t WriteOnlySocket::write(void* data, size_t length) {
    int ret = send(sockfd, data, length, 0);
    if (ret == -1) {
        if (errno == EWOULDBLOCK) {
            return 0;
        } else {
            throw std::runtime_error(strerror(errno));
            return 0;
        }
    }
    return ret;
}

bool WriteOnlySocket::try_connect() {
    if (socket_is_connected) return true;
    if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0) {
        printf("CONNECTED!\n");
        socket_is_connected = true;
        failed_connection_attempts = 0;
        return true;
    } else {
        switch (errno) {
            case EALREADY: socket_is_connected = true; return true; break;
            case ECONNREFUSED: //Fallthrough
            case EINPROGRESS: failed_connection_attempts++; break;
            default: throw std::runtime_error(strerror(errno)); break;
        }
        return false;
    }
}

void WriteOnlySocket::disconnect() {
    socket_is_connected = false;
    close(sockfd);
}

WriteOnlySocket::~WriteOnlySocket() {
    disconnect();
}

int WriteOnlySocket::get_failed_connection_attempts() {
    return failed_connection_attempts;
}
