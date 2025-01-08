FLAGS = -Wall -Wextra -Werror -g3
all: $(NAME)
			@make -C $(LIBFT) all
			@make -C $(FT_PRINTF) all
re: fclean all
.PHONY: all libft ft_printf clean fclean re
