#pragma once

#include <string>
#include <iostream>
#include <map>

class Client;

class Channel
{
    private:
        Channel();
        std::map<std::string, Client> _userList;
        std::map<std::string, Channel> _channels;
        std::string _nameChannel;
        std::string _password;

    public:
        Channel(std::string nameChannel):
        Channel(std::string nameChanel, std::string password);
        ~Channel();
        std::string getName() const;
        void removeUserFromChannel(const std::string &nameChannel, const std::string &username);
        void removeUser(const std::string &username);
};