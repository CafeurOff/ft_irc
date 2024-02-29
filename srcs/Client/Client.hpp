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

class Client{
    public:
        Client(int fd);
        ~Client();
        std::string const &getNickname() const;
        std::string const &getUsername() const;
        int getFd() const;
        void setNickname(const std::string &nickname);
        void setUsername(const std::string &username);
        static Client* findClientByNickname(const std::string& nickname);
    private:
        std::string _nickname;
        std::string _username;
        int _fd;

};