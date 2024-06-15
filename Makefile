# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/12 10:15:37 by ncasteln          #+#    #+#              #
#    Updated: 2024/06/15 11:42:40 by fahmadia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

# @-Wconversion - prevent implicit cast when accuracy is lost.
# @-Wsign-conversion - prevent implicit change of sign.
# @-Wshadow - warns about variable declarations that shadow a variable declared in an outer scope.
CXXFLAGS	=	-std=c++98 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion #-Werror

VPATH		=	src
SRC			=	main.cpp \
				Http.cpp \
				Server.cpp \
				Location.cpp \
				test.cpp

HEADERS		=	./src/Http.hpp \
				./src/Server.hpp \
				./src/Location.hpp \

INCLUDE		=	-I./include/

OBJS_PATH	=	objs
OBJS		=	$(SRC:%.cpp=$(OBJS_PATH)/%.o)

# ----------------------------------------------------------------------- BASIC
all: $(NAME)

$(NAME): $(OBJS)
	c++ $(CXXFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)

$(OBJS_PATH)/%.o: %.cpp $(HEADERS)
	mkdir -p $(OBJS_PATH)
	c++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJS_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

# ----------------------------------------------------------------------- NGINX
nginx: nginx-img
	docker run --detach --name $@ -p 8000:8000 $<;
	@if [ $$(docker ps -a --filter "status=running" | grep nginx | wc -l) -ne 0 ]; then \
		echo "$(G)* nginx is running$(W)"; \
	else \
		echo "$(R)* nginx exited$(W)"; \
	fi

nginx-img:
	cd nginx && docker build -t $@ .

nginx-rm:
	docker stop nginx;
	docker rm nginx;
	docker rmi nginx-img;

G	=	\033[0;32m
Y	=	\033[0;33m
B	=	\033[0;34m
R	=	\033[0;31m
W	=	\033[0m
N	=	\033[1;30m

.PHONY: all clean fclean re nginx nginx-img nginx-rm
