#pragma once

#include <wo_socket.hpp>
#include <string>

/* Simple adapter from a raw WriteOnlySocket to 
 * a state-aware motion server communication line 
 * TODO: More intelligent message buffer/send_u16 method
 */
class MotionServerConnection {
    public:
        // ID refers to the id string the motion server looks for as first message
        MotionServerConnection(std::string ip, int port, std::string id);

        // Call this to check connection and attempt reconnection if possible
        bool keepalive();

        // Send an unsigned short to the motion server
        void send_u16(unsigned short input);
    private:

        // Indicates whether the id has been sent or not.
        bool has_sent_id;
        std::string id;
        WriteOnlySocket sock;
};
