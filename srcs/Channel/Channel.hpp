#pragma once

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <vector>
#include <map>
#include <ctime>

class Client;

class Channel {
    public:
        Channel(const std::string& name);
        ~Channel();
        const std::string &getName() const;
        static void sendMessage(Client* client, const std::string& msg);
        static void createChannel(const std::string& name, Client* creator);
        void sendNumericResponse(Client* client, const std::string& code, const std::string& param1, const std::string& param2);
        void kick(Client* creator, const std::string& targetNickname);
        void sendAll(const std::string& message);
        void addUser(Client* user);
        void removeUser(Client* user);
        void invite(Client* sender, const std::string& targetNickname);
        void topic(Client* sender, const std::string& newTopic);
    private:
        std::string _name;
        std::string _password;
        std::string _topic;
        bool _inviteOnly;
        bool _restrictTopic;
        bool _limitUser;
        bool _passwordUse;
        int _nUser;
        std::map<std::string, Client*> _operators;
        std::map<std::string, Client*> _regulars;
        

};