# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsouza-r <lsouza-r@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/11 17:34:18 by cshingai          #+#    #+#              #
#    Updated: 2025/01/08 18:40:06 by lsouza-r         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
FLAGS = -Wall -Wextra -Werror -g3
LIBFT = libs/libft
FT_PRINTF = libs/ft_printf
LIBS = $(LIBFT)/libft.a $(FT_PRINTF)/libftprintf.a
HEADERS = -I $(LIBFT) -I ./include

# source files
SRCS = $(addprefix srcs/, \
			main.c \
			signal.c \
			heredoc.c \
			utils/utils.c \
			utils/list_utils.c \
			utils/list_utils2.c \
			tokenizer/tokenizer.c \
			tokenizer/token_states.c \
			tokenizer/token_states_quotes.c \
			tokenizer/validation.c \
			tokenizer/token_utils.c \
			env/env_list.c \
			env/env_utils.c \
			env/env_print.c \
			executor/executor.c \
			executor/exec_single_cmd.c \
			executor/handle_pipe.c \
			executor/handle_redir.c \
			executor/exec_cmd.c \
			executor/executor_clean.c \
			builtin/export.c \
			builtin/export_utils.c \
			builtin/export_utils2.c \
			builtin/exit.c\
			builtin/unset.c \
			builtin/pwd.c \
			builtin/echo.c \
			builtin/change_directory.c\
			builtin/exec_builtin.c\
			expander/expander.c \
			expander/expander2.c \
			parsing_tree/parsing-tree.c \
			parsing_tree/hunt_redir.c \
		)

OBJ = $(SRCS:srcs/%.c=obj/%.o)

all: $(NAME)

$(NAME): libft ft_printf ${OBJ}
			@cc $(FLAGS) $(OBJ) $(LIBS) -lreadline -o $@
			@echo "compiling $(NAME)"

# building libraries
libft:
			@make -C $(LIBFT) all

ft_printf:
			@make -C $(FT_PRINTF) all

# compiling objects files
obj/%.o: srcs/%.c ./include/minishell.h
			@mkdir -p $(dir $@)
			@cc $(FLAGS) $(HEADERS) -c $< -o $@
			@echo "compiling $<"

# cleanning up objects files
clean:
			@echo "removing objects"
			@rm -rf obj
			@make clean -C $(LIBFT)
			@make clean -C $(FT_PRINTF)

# cleaning up executables
fclean:	clean
			@echo "removing executables"
			@rm -rf $(NAME)
			@echo "removing libs"
			@make fclean -C $(LIBFT)
			@make fclean -C $(FT_PRINTF)
			@echo "everything was removed 🧹🗑️"

re: fclean all

.PHONY: all libft ft_printf clean fclean re
