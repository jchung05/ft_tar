# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sjuery <sjuery@student.42.us.org>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/01/28 20:44:30 by sjuery            #+#    #+#              #
#    Updated: 2018/01/28 20:49:38 by sjuery           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ft_archive

SRC		= 	ft_archive.c \
			header.c \
			main.c \

OBJ 	= $(SRC:.c=.o)
CFLAG	= -Wall -Wextra -Werror -g
.SILENT:

all: $(NAME)

$(NAME): $(OBJ)
	printf '\033[31m[...] %s\n\033[0m' "Creating Ft_Archiver"
	gcc $(CFLAG) $^ -o $(NAME)
	printf '\033[32m[ ✔ ] %s\n\033[0m' "Created Ft_Archiver"

clean:
	printf '\033[31m[...] %s\n\033[0m' "Cleaning Ft_Archiver"
	/bin/rm -f *.o
	printf '\033[32m[ ✔ ] %s\n\033[0m' "Cleaned Ft_Archiver"

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all test
