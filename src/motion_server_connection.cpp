#include <motion_server_connection.hpp>

MotionServerConnection::MotionServerConnection(std::string ip, int port, std::string id) : sock(ip, port), id(id) {}

bool MotionServerConnection::keepalive() {
    if (sock.keepalive()) {
        if (!has_sent_id) {
            sock.write((void*)id.c_str(), id.size());
            has_sent_id = true;
        } 
        return true;
    } else {
        has_sent_id = false;
        return false;
    }
}

void MotionServerConnection::send_u16(unsigned short input) {
    char message_buf[64];
    snprintf(message_buf, 64, "%hu\n", input);
    sock.write(message_buf, strlen(message_buf));
}
