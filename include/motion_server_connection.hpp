#pragma once

#include <wo_socket.hpp>
#include <string>

class MotionServerConnection {
    public:
        MotionServerConnection(std::string ip, int port, std::string id);
        bool keepalive();
        void send_u16(unsigned short input);
        ~MotionServerConnection();
    private:
        WriteOnlySocket sock;
        bool has_sent_id;
        std::string id;
};
