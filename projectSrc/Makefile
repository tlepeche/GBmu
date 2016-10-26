# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: barbare <barbare@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/26 12:39:41 by barbare           #+#    #+#              #
#    Updated: 2016/10/26 12:46:13 by barbare          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=GBmu

# Src for debugger
SRCS +=	src/db/debugger.cpp

# Src for gameboy
SRCS += src/gb/gameboy.cpp

# Srcs common
SRCS += src/main.cpp

OBJS = $(SRCS:.cpp=.o)

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
