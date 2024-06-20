# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/12 10:15:37 by ncasteln          #+#    #+#              #
#    Updated: 2024/06/20 20:23:32 by fahmadia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

# @-Wconversion - prevent implicit cast when accuracy is lost.
# @-Wsign-conversion - prevent implicit change of sign.
# @-Wshadow - warns about variable declarations that shadow a variable declared in an outer scope.
CXXFLAGS	=	-std=c++98 -g -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion #-Werror

VPATH		=	src src/sockets/socket src/sockets/listening-socket src/sockets/connected-socket src/test src/http-server src/exception

SRC			=	main.cpp \
				Http.cpp \
				Server.cpp \
				Location.cpp \
				Socket.cpp \
				ListeningSocket.cpp \
				ConnectedSocket.cpp \
				HttpServer.cpp \
				Exception.cpp \
				memoryTest.cpp \

HEADERS		=	./src/Http.hpp \
				./src/Server.hpp \
				./src/Location.hpp \
				./src/error.hpp \
				./src/sockets/socket/Socket.hpp \
				./src/sockets/listening-socket/ListeningSocket.hpp \
				./src/sockets/connected-socket/ConnectedSocket.hpp \
				./src/http-server/HttpServer.hpp \
				./src/exception/Exception.hpp \
				./src/test/test.hpp \

INCLUDE		=	-I./include/ -I./src -I./src/sockets/socket -I./src/sockets/listening-socket -I./src/sockets/connected-socket -I./src/http-server -I./src/exception -I./src/test

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
