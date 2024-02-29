#include "Client.hpp"

Client::Client(int fd){
    _fd = fd;
}

Client::~Client(){
    close(_fd);
}

std::string const& Client::getNickname() const{
    return (_nickname);
}

std::string const& Client::getUsername() const{
    return (_username);
}

int Client::getFd() const{
    return (_fd);
}

void Client::setNickname(const std::string &nickname){
    _nickname = nickname;
}

void Client::setUsername(const std::string &username){
    _username = username;
}


