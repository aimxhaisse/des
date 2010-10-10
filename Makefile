CFLAGS 		= -W -Wall -pedantic -ansi -D_XOPEN_SOURCE --std=c99 -DDEBUG
SRCS  		= main.c mode.c des.c tools.c
OBJS		= $(SRCS:.c=.o)

all:		$(OBJS)
		gcc -o des $(OBJS)

clean:
		rm -f $(OBJS)

fclean:		clean
		rm -f des

re:		fclean all
