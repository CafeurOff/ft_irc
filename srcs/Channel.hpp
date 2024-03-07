#pragma once

#include <string>
#include <iostream>
#include <map>

class Client;

class Channel
{
    private:
        std::string _name;
        std::string _password;
        std::string _topic;
        bool _inviteOnly;
        bool _restrictTopic;
        bool _limitUser;
        bool _passwordUse;
        int _nUser;
        int _nUserLimit;
        std::map<std::string, Client*> _operators;
        std::map<std::string, Client*> _regulars;
        std::map<std::string, Client*> _invited;

    public:
        Channel(std::string name):
        Channel(std::string name, std::string password);
        ~Channel();

        const std::string &getName() const;
        
        static void sendMessage(Client* client, const std::string& msg);
        static void createChannel(const std::string& name, Client* creator);
        void sendNumericResponse(Client* client, const std::string& code, const std::string& param1, const std::string& param2);
        
        void sendAll(const std::string& message);
        
        void kick(Client* creator, const std::string& targetNickname);
        void addUser(Client* user);
        void removeUser(Client* user);
        void invite(Client* sender, const std::string& targetNickname);
        void topic(Client* sender, const std::string& newTopic);

        void checkMode(std::string **mess);
        void modifMode(char modeSign, char modeChar, const std::string &param);
};