CC = gcc
RM = rm -f
NAME = ssu_sdup
HELP = help
MD5 = fmd5
SHA1 = fsha1
INCLUDE = ./includes/header.h
SRC = ./src/split.c ./src/ssu_sdup.c 
HELP_SRC = ./src/ssu_help.c
MD5_INCLUDE = ./includes/md5_header.h
MD5_SRC = ./src/queue.c ./src/ssu_find-md5.c
SHA1_INCLUDE = ./includes/sha1_header.h
SHA1_SRC = ./src/queue.c
OBJS = $(SRC:.c=.o)

all : $(NAME) $(HELP) $(MD5)
$(NAME) :
	$(CC)	$(SRC) -o $(NAME) -include $(INCLUDE)

$(HELP) :
	$(CC)	$(HELP_SRC) -o $(HELP)

$(MD5) :
	$(CC)	$(MD5_SRC) -o $(MD5) -include $(MD5_INCLUDE)

clean :
	$(RM) *.o

fclean :
	$(RM) $(NAME) $(HELP) $(MD5)

re : fclean all
