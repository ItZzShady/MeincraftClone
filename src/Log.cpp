#include "Log.h"
#include <iostream>

namespace Log {
    // Private functions and variables
    namespace {
        void _logMessage(std::string suffix, std::string message) {
            std::cout << suffix << message << std::endl;
        }
    }
    
    void Print(std::string message, MessageType messageType) {
        switch (messageType) {
            case OK : 
                _logMessage("", message);
                break;
            case INFO : 
                _logMessage("[INFO]: ", message);
                break;
            case ERROR:
                _logMessage("[ERROR]: ", message);
                break;
            case WARNING: 
                _logMessage("[WARNING]: ", message);
                break;    
        }
    }
    void Print(std::string message, std::string info, MessageType messageType) {
        
        message = message + info;
        
        switch (messageType) {
            case OK : 
                _logMessage("", message);
                break;
            case INFO : 
                _logMessage("[INFO]: ", message);
                break;
            case ERROR:
                _logMessage("[ERROR]: ", message);
                break;
            case WARNING: 
                _logMessage("[WARNING]: ", message);
                break;    
        }
    }
}