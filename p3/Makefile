CC = gcc
RM = rm -f
NAME = ssu_sfinder
INCLUDE = ./includes/ssu_sfinder.h
SRC = ./src/split.c ./src/ssu_sfinder.c ./src/list.c ./src/queue.c
OBJS = $(SRC:.c=.o)

all : $(NAME) 
$(NAME) :
	$(CC)	-g $(SRC) -o $(NAME) -include $(INCLUDE) -lcrypto -lpthread

clean :
	$(RM) *.o

fclean :
	$(RM) $(NAME) 

re : fclean all
