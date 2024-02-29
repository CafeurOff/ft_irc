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
        void Channel::addChannel(std::string const &name, Client* client)
        {
            Channel(name) channel1;
            if (_channel.find(name) != _channel.end())
            {
                //ERROR
                return ;
            }
            _channels.insert(std::pair(name, channel1));
            channel1.createChannel(name, client);
        }

        void Channel::addChannelPassword(std::string const &name, std::string const &password, Client* client)
        {
            Channel(name, password) channel1;
            if (_channel.find(name) != _channel.end())
            {
                //ERROR
                return ;
            }
            _channels.insert(std::pair(name, channel1));
            channel1.createChannel(name, client);
        }

    private:
        std::map<std::string, Channel> _channel;
        std::map<std::string, Client> _client;
};


