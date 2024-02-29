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
