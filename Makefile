CFLAGS 		= -W -Wall -pedantic -ansi
SRCS  		= main.c
OBJS		= $(SRCS:.c=.o)

all:		$(OBJS)
		gcc -o des $(OBJS)

clean:
		rm -f $(OBJS)

fclean:		clean
		rm -f des

re:		fclean all
