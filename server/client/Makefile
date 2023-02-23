NAME		=	client

CC			=	g++ 

RM			=	rm -f

SRCS		=	./main.cpp					\
				./Clients/TCPClient.cpp		\
				./Clients/UDPClient.cpp		\
				./Data/ClientData.cpp		\
				./Server/ClientServer.cpp	\

OBJS		=	$(SRCS:.cpp=.o)

CPPFLAGS 	=	-I -pthread -I./Include/
# CPPFLAGS += -Wall -Wextra

LDFLAGS		+=	-L/usr/local/lib -lboost_system -lpthread

all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) $(LDFLAGS) -o $(NAME) 

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
