#pragma once

#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <ctime>

class Client;
class Channel;

class server
{
    public:

    private:
        std::vector<std::string> _channel;
        std::map<std::string, std::string> _client;
};