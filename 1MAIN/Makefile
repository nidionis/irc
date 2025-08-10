# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lahlsweh <lahlsweh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/27 13:48:14 by lahlsweh          #+#    #+#              #
#    Updated: 2025/08/10 14:37:23 by lahlsweh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler settings
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I./include -g3 #-fsanitize=leak,address,undefined,null,float-divide-by-zero
DEBUG_FLAGS = -g3 -O0 -DDEBUG

# Project settings
NAME = ircserv
SRC_DIR = src
OBJ_DIR = obj
DEBUG_DIR = debug
INCLUDE_DIR = include

# Source files (update these as you add more source files)
SRCS =	capabilities.cpp \
		Channel.cpp \
		Client.cpp \
		handle.cpp \
		handle2.cpp \
		main.cpp \
		Server_Channel.cpp \
		Server_Client.cpp \
		Server_poll.cpp \
		Server.cpp \
		utils_strings.cpp \

# Object files
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEBUG_OBJS = $(SRCS:%.cpp=$(DEBUG_DIR)/%.o)

# Default target
all: $(NAME)

# Debug target
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(NAME)_debug

# Linking the executable
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Linking the debug executable
$(NAME)_debug: $(DEBUG_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compiling debug source files
$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(DEBUG_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(DEBUG_DIR)

# Full clean (including executable)
fclean: clean
	rm -f $(NAME)
	rm -f $(NAME)_debug

# Rebuild the project
re: fclean all

# Debug rebuild
debug_re: fclean debug


# Ensure these targets don't conflict with files
.PHONY: all clean fclean re debug debug_re