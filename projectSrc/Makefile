# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: barbare <barbare@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/26 12:39:41 by barbare           #+#    #+#              #
#    Updated: 2016/10/26 14:59:05 by barbare          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=GBmu

CC=clang++
CFLAGS = -Wall -Werror -Wextra

# Src for debugger
DIR_DB := src/db/
SRCS  += $(DIR_DB)debugger.cpp

# Src for gameboy
DIR_GB := src/gb/
INC_GB := inc/gb/
SRCS  += $(DIR_GB)gameboy.cpp

# Srcs common
DIR_COM := src/
INC_COM := inc/
SRCS += $(DIR_COM)main.cpp

OBJS = $(patsubst src/%.cpp, obj/%.o,$(SRCS))

INC_FILES =

RM = rm -f
MKDIR = mkdir -p

all: $(NAME)

obj/%.o: src/%.cpp
	$(MKDIR) $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(NAME): $(OBJS)
	$(CC) -O3 -o $@ $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all
