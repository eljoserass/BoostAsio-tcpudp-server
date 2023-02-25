NAME	= server

CC	= g++

RM	= rm -f

SRCS	=	./Game/Game.cpp 		\
			./Game/GameManager.cpp	\
			./Player/Player.cpp		\
			./Room/Room.cpp			\
			./Room/RoomManager.cpp	\
			./Servers/TCPServer.cpp \
			./Servers/UDPServer.cpp \
			./main.cpp 

OBJS	= $(SRCS:.cpp=.o)

CPPFLAGS = -I./Include/Room -I./Include/Player -I./Include/Game -I./Include/Servers
# CPPFLAGS += -Wall -Wextra

LDFLAGS += -L/usr/local/lib -lboost_system -lpthread


all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
