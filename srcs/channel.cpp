#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string nameChannel) : _nameChannel(nameChannel)
{

}

Channel::~Channel()
{

}

std::string Channel::getName() const;
{
    return (_nameChannel)
}

void Channel::removeUser(const std::string &username)
{
    _userList.erase(username);
}