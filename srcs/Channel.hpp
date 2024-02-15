#pragma once

#include<string>

class Channel
{
	public:
		Channel();
		Channel(Channel const& copy);
		~Channel();
		Channel& operator=(Channel const& copy);
		//set & remove limit of users
		//set & remove passwords
		//set & remove restriction topic cmd
		//give & take operator privilege
		//
	
	private:
		bool			_invite;//true channel in invite only / else false
		bool			_topiccmd;//true command TOPIC is restricted to operator / else false
		std::string		_passwrd;//empty if no password needed
		unsigned int	_limit;//0 if there is no limit of users connected to the channel
		//array of users
		//array of users with operator privilege
};