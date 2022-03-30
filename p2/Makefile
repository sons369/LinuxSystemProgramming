CC = gcc
RM = rm -f
NAME = ssu_sdup
HELP = help
INCLUDE = ./includes
SRC = ./src/split.c ./src/ssu_sdup.c 
HELP_SRC = ./src/ssu_help.c
OBJS = $(SRC:.c=.o)
all : $(NAME) $(HELP)
$(NAME) :
	$(CC)	$(SRC) -o $(NAME) -I $(INCLUDE)

$(HELP) :
	$(CC)	$(HELP_SRC) -o $(HELP)

clean :
	$(RM) *.o

fclean :
	$(RM) $(NAME) $(HELP)

re : fclean all
