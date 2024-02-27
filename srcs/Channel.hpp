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
        std::string _nameChannel;

    public:
        Channel(std::string nameChannel):
        ~Channel();
        std::string getName() const;
        void removeUser(const std::string &username);

};