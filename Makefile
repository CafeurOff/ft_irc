# FT_IRC # Internet relay chat between a client and server

NAME = ircserv
FLAGS = -Wall -Werror -Wextra -std=c++98
CC = c++

# Sources Files
SRCS_FOLDER = srcs/
SRCS = srcs/main.cpp 
OBJS = $(SRCS:.cpp=.o)

# Rules
all:	$(NAME)

$(NAME):	$(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled succesfully!$(RESET)"

%.o:	%.cpp
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@echo "$(RED)$(RED) objects deleted successfully!$(RESET)"

fclean:	clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) deleted successfully!$(RESET)"

re:	fclean all

.PHONY:	all clean fclean re
