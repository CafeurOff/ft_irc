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

class Client 
{
    private:
        Client();
        int _fdClient;
        std::string _nickname;
        std::string _username;

    public:
        Client(int fd);
        ~Client();
        std::string const &getNickname() const;
        std::string const &getUsername() const;
        int getFd() const;
        void setNickname(const std::string &nickname);
        void setUsername(const std::string &username);
};