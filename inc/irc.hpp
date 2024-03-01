#pragma once

# include <stdlib.h>
# include <cstdio>
# include <unistd.h>
# include <iostream>
# include <iomanip>
# include <string>
# include <cstring>
# include <cstdlib>
# include <map>
# include <algorithm>
# include <signal.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sstream>
# include <signal.h>
# include <vector>
# include <netdb.h>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

# include <poll.h>

void	ft_launch_server(char **av);