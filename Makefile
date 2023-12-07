CC = gcc
CFLAGS = -Wall -Wextra -DTIMES

LIB_SRCS = srcs/client.c srcs/server.c srcs/ipc.c srcs/file.c srcs/co_scenario.c srcs/so_scenario.c srcs/arrange.c
MAIN_SRC = srcs/mytest.c

SOURCE   = data/p0.dat data/p1.dat data/p2.dat data/p3.dat
RESULT	 = data/*

LIB_OBJS	= $(LIB_SRCS:.c=.o)
MAIN_OBJ	= $(MAIN_SRC:.c=.o)
INCLUDES	= -I ./includes
NAME	= libmytest.a
EXE		= mytest

RM		= rm -f
AR		= ar rcs

all :		setup $(NAME) $(EXE)

setup :		$(NAME) $(EXE)
			@mkdir -p data

$(NAME)	:	$(LIB_OBJS)
			$(AR) $@ $^

$(EXE): $(MAIN_OBJ) $(NAME)
			$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(MAIN_OBJ) -L. -lmytest

.c.o:
			${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@
			
clean :	
			$(RM) $(LIB_OBJS) $(MAIN_OBJ)

fclean :	clean
			$(RM) $(NAME) $(EXE) $(SOURCE) $(RESULT)

re :		fclean all

.PHONY :	all clean fclean re
