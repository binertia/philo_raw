NAME			=	philo
CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror
INCLUDES_DIR	=	includes/
INCLUDES		=	$(addprefix $(INCLUDES_DIR), philosophers.h)
SRCS_DIR		=	srcs/
SRCS			=	$(addprefix $(SRCS_DIR), \
					alone_philo.c \
					atonum.c \
					fork_cycle.c \
					free_exit.c \
					get_input.c \
					main.c \
					print.c \
					thread.c \
					time.c \
					)

OBJS			=	$(SRCS:%.c=%.o)

.PHONY: 			all clean fclean re

all:				$(NAME)

$(NAME): 			$(OBJS)
					$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: 				%.c	$(INCLUDES)
					$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

clean:
					rm -rf $(OBJS)

fclean: 			clean
					rm -rf $(NAME)

re: 				fclean all
