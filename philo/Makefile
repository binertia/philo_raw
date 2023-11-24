NAME         = philo
CC           = cc
CFLAGS       = -Wall -Wextra -Werror
LIBS         = -lpthread

INCLUDES_DIR = includes/
INCLUDES_FILES = philosophers.h
INCLUDES     = $(addprefix $(INCLUDES_DIR), $(INCLUDES_FILES))

SRCS_DIR     = srcs/
SRCS_FILES   = alone_philo.c \
               atonum.c \
               fork_cycle.c \
               free_exit.c \
               get_input.c \
               main.c \
               print.c \
               thread.c \
               time.c

SRCS         = $(addprefix $(SRCS_DIR), $(SRCS_FILES))

OBJS_DIR     = objs/
OBJS         = $(addprefix $(OBJS_DIR), $(notdir $(SRCS:.c=.o)))

.PHONY: all clean fclean re

all: 			$(NAME)

$(NAME):		$(OBJS)
				$(CC) $(CFLAGS) $(LIBS) $^ -o $@

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c | $(OBJS_DIR)
				$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

$(OBJS_DIR):
				mkdir -p objs

clean:
				rm -rf $(OBJS)

fclean: clean
				rm -rf $(NAME)

re: fclean all
