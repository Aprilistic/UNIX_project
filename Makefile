CC = gcc
CFLAGS = -Wall -Wextra -Werror -DTIMES -DDATA_SIZE=$(DATA_SIZE)

DATA_SIZE = "(1024 * 1024)"

LIB_SRCS = srcs/client.c srcs/server.c srcs/ipc.c srcs/file.c srcs/co_scenario.c srcs/so_scenario.c srcs/arrange.c
MAIN_SRC = srcs/mytest.c

SOURCE   = data/input0.dat data/input1.dat data/input2.dat data/input3.dat
RESULT   = data/* res/*

LIB_OBJS = $(LIB_SRCS:.c=.o)
MAIN_OBJ = $(MAIN_SRC:.c=.o)
INCLUDES = -I ./includes
NAME     = libmytest.a
EXE      = mytest

RM       = rm -f
AR       = ar rcs

all :    setup $(NAME) $(EXE)

setup :  $(NAME) $(EXE)
		@mkdir -p data

$(NAME) :    $(LIB_OBJS)
			$(AR) $@ $^

$(EXE): $(MAIN_OBJ) $(NAME)
		$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(MAIN_OBJ) -L. -lmytest

.c.o:
		${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@

clean : 
		$(RM) $(LIB_OBJS) $(MAIN_OBJ)

fclean :    clean
			$(RM) $(NAME) $(EXE) $(SOURCE) $(RESULT)

re :    fclean all

test: DATA_SIZE = "(64)"
test: re

.PHONY :    all clean fclean re test
