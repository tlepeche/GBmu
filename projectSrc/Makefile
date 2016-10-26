# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: barbare <barbare@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/26 12:39:41 by barbare           #+#    #+#              #
#    Updated: 2016/10/26 13:53:37 by barbare          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=GBmu

# Src for debugger
DIR_DB := src/db/
SRCS_DB  += debugger.cpp

OBJS = $(patsubst $(DIR_DB)/%.c, obj/%.o,$(SRCS))

# Src for gameboy
DIR_GB := src/db/
SRCS_GB  += src/gb/gameboy.cpp

OBJS += $(patsubst $(DIR_GB)/%.c, obj/%.o,$(SRCS))

# Srcs common
DIR_COM := src/db/
SRCS_COM += src/main.cpp

OBJS += $(patsubst $(DIR_COM)/%.c, obj/%.o,$(SRCS))


RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all $(NAME)
