#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string nameChannel) : _nameChannel(nameChannel)
{

}

Channel::Channel(std::string nameChannel, std::string password) : _nameChannel(nameChannel), _password(password)
{

}

Channel::~Channel()
{

}

std::string Channel::getName() const;
{
    return (_nameChannel)
}

void removeUser(const std::string& username) {
        _userList.erase(username);
    }

void Channel::removeUserFromChannel(const std::string &nameChannel, const std::string &username)
{
    std::map<std;;string, Channel>::iterator it = _channels.find(channelName);
    if (it != _channels.end())
        it->second.removeUser(username);
}