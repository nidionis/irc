# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/10 16:30:54 by lahlsweh          #+#    #+#              #
#    Updated: 2025/06/25 13:24:39 by lahlsweh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = poll_test

COMPILATOR = c++

CFLAGS = -std=c++98 -Wall -Wextra -Werror -I./includes -pedantic -g3 -fsanitize=leak,address,undefined,integer,null,float-divide-by-zero
OFLAGS = -std=c++98 -Wall -Wextra -Werror -I./includes -pedantic -g3 -fsanitize=leak,address,undefined,integer,null,float-divide-by-zero

FILES = main \

SOURCES = $(foreach f, $(FILES), ./$(f))
OBJECTS = $(foreach f, $(FILES), ./$(f).o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(COMPILATOR) $(CFLAGS) -o $@ $^

./objects/%.o: ./%.cpp
	@mkdir -p $(dir $@)
	$(COMPILATOR) $(OFLAGS) -c $< -o $@

clean:
	rm -rf ./*.o

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re