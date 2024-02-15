#pragma once

#include<string>

class Client
{
	public:
		Client();
		Client(Client const& copy);
		~Client();
		Client& operator=(Client const& copy);
		//set username
		//set & reset nickname
		//send private message
		//send message to a channel
	
	private:
		std::string		_username;
		std::string		_nickname;
		int				_fd;
};