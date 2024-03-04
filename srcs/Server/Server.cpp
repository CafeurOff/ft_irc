#include "Server.hpp"

//method qui cherche en param nickname et qui renvoie fd si il est trouvé, -1 sinon

int Server::findFdByNickname(const std::string& nickname) {
    std::map<std::string, Client>::iterator it = _clients.find(nickname);
    if (it != _clients.end()) {
        // Le client a été trouvé dans la map
        return it->second.getFd();
    } else {
        // Le client n'a pas été trouvé dans la map
        return -1;
    }
}

// method pour cherche un fd dans une map, si il existe renvoie fd sinon -1

int Server::findFd(int fd) {
    for (std::map<std::string, Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        if (it->second.getFd() == fd) {
            // Le fd a été trouvé dans la map
            return fd;
        }
    }
    // Le fd n'a pas été trouvé dans la map
    return -1;
}