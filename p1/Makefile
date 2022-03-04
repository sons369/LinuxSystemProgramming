CC = gcc
RM = rm -f
NAME = ssu_sindex
INCLUDE = ./includes
SRC = ./src/*.c
OBJS = $(SRC:.c=.o)
all : $(NAME)
$(NAME) :
	$(CC)	$(SRC) -o $(NAME) -I $(INCLUDE)

clean :
	$(RM) *.o

fclean :
	$(RM) $(NAME)

re : fclean all
