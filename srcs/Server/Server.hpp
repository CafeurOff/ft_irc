#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <map>
#include "Client.hpp"
#include "Channel.hpp"

class Server{
    public:
        int findFdByNickname(const std::string& nickname);
    private:
        std::map<std::string, Client> _clients;
}