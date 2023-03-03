NAME	= server

CC	= g++

RM	= rm -f

SRCS	=	./Game/Game.cpp 		\
			./Game/GameManager.cpp	\
			./Server/UDPServer.cpp \
			./main.cpp 

OBJS	= $(SRCS:.cpp=.o)

CPPFLAGS = -I./Include/Game -I./Include/Server
# CPPFLAGS += -Wall -Wextra

LDFLAGS += -L/usr/local/lib -lboost_system -lpthread


all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) $(LDFLAGS) -g -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
