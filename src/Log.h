#pragma once
#ifndef LOG_H
#define LOG_H

#include <string>

enum MessageType {
    OK, ERROR, WARNING, INFO
};

namespace LOG {
    // Print`s log the message 
    void Print(std::string message, MessageType messageType);
    void Print(std::string message, std::string info, MessageType messageType);
};
#endif