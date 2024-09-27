# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/12 10:15:37 by ncasteln          #+#    #+#              #
#    Updated: 2024/08/18 21:58:23 by nnabaeei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME     := webserv
CXX      := g++
CXXFLAGS := -Wall -Wextra  -std=c++98 -g #-Werror
CXXFLAGS += -Wshadow -Wno-shadow #-fsanitize=address

# Directories
SERVER_LOG := ./src/server.log
SRC_DIR := src
OBJ_DIR := objs
EXTRA_INC_DIR := include

# Find all .cpp files in the SRC directory and subdirectories
CPP_FILES := $(shell find $(SRC_DIR) -type f -name '*.cpp')

# Find all directories containing .hpp files
INCLUDE_DIRS := $(shell find $(SRC_DIR) -type f -name '*.hpp' -exec dirname {} \; | sort -u)
INCLUDE_DIRS += $(shell find $(EXTRA_INC_DIR) -type f -name '*.h' -exec dirname {} \; | sort -u)

# Add include flags for each directory
INCLUDES := $(addprefix -I,$(INCLUDE_DIRS))

# Generate object file names, preserving directory structure
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_FILES))

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) -c -o $@ $< $(CXXFLAGS) $(INCLUDES)

$(NAME): $(OBJ_FILES)
	@$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(NAME) $(LDFLAGS)
	@echo "$(ORG)----- $(RESET)$(GREEN)$(NAME)$(RESET)$(ORG) as exe file is created!-----$(RESET)"

clean:
	@rm -rf $(OBJ_DIR) $(SERVER_LOG)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

BLUE    = \033[38;5;4m
GREEN   = \033[38;5;2m
ORG     = \033[38;5;214m
RED     = \033[38;5;196m
RESET   = \033[0m

# NAME		=	webserv

# # @-Wconversion - prevent implicit cast when accuracy is lost.
# # @-Wsign-conversion - prevent implicit change of sign.
# # @-Wshadow - warns about variable declarations that shadow a variable declared in an outer scope.
# CXXFLAGS	=	-std=c++98 -g -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion #-Werror

# VPATH		=	src \
# 				src/sockets/socket \
# 				src/sockets/listening-socket \
# 				src/sockets/connected-socket \
# 				src/test \
# 				src/server \
# 				src/exception \
# 				src/parsing

# PARSING_CPP	=	Parser.cpp \
# 				HttpConf.cpp \
# 				ServerConf.cpp \
# 				LocationConf.cpp
# PARSING_HPP	=	./src/parsing/Parser.hpp \
# 				./src/parsing/HttpConf.hpp \
# 				./src/parsing/ServerConf.hpp \
# 				./src/parsing/LocationConf.hpp

# SOCKETS_CPP	=	Socket.cpp \
# 				ListeningSocket.cpp \
# 				ConnectedSocket.cpp
# SOCKETS_HPP	=	./src/sockets/socket/Socket.hpp \
# 				./src/sockets/listening-socket/ListeningSocket.hpp \
# 				./src/sockets/connected-socket/ConnectedSocket.hpp \

# SERVER_CPP	=	Server.cpp \
# 				Exception.cpp
# SERVER_HPP	=	./src/server/Server.hpp \
# 				./src/exception/Exception.hpp

# # files to tidy up
# EXTRA_CPP	=	memoryTest.cpp
# EXTRA_HPP	=	./src/error.hpp \
# 				./src/test/test.hpp \

# SRCS		=	main.cpp \
# 				$(PARSING_CPP) \
# 				$(SOCKETS_CPP) \
# 				$(SERVER_CPP)
# HEADERS		=	$(PARSING_HPP) \
# 				$(SOCKETS_HPP) \
# 				$(SERVER_HPP) \
# 				$(EXTRA)

# INCLUDE		=	-I./include/ \
# 				-I./src \
# 				-I./src/sockets/socket \
# 				-I./src/sockets/listening-socket \
# 				-I./src/sockets/connected-socket \
# 				-I./src/server \
# 				-I./src/exception \
# 				-I./src/test \
# 				-I./src/parsing

# OBJS_PATH	=	objs
# OBJS		=	$(SRCS:%.cpp=$(OBJS_PATH)/%.o)

# # Verbosity can be defined like 'make VERBOSE=1;' The default by doing 'make' is 0
# # Only three levels are allowed:
# #	0 = the minimal messages we want to show, default behavior of the program.
# #	1 = additional useful messages, like a setting felt back to a default value
# #		(ex. the server block in parsing has no port, so the automatic is 80; this is notified).
# #	2 = unuseful information, like the steps performed by the parsing, useful only
# #		for debugging pourposes.
# VERBOSE		?=	0

# # ----------------------------------------------------------------------- BASIC
# all: $(NAME)

# $(NAME): $(OBJS)
# 	c++ $(CXXFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)

# $(OBJS_PATH)/%.o: %.cpp $(HEADERS)
# 	mkdir -p $(OBJS_PATH)
# 	c++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@ -DVERBOSE=$(VERBOSE)

# clean:
# 	rm -rf $(OBJS_PATH)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all

# # ----------------------------------------------------------------------- NGINX
# nginx: nginx-img
# 	docker run --detach --name $@ -p 8000:8000 $<;
# 	@if [ $$(docker ps -a --filter "status=running" | grep nginx | wc -l) -ne 0 ]; then \
# 		echo "$(G)* nginx is running$(W)"; \
# 	else \
# 		echo "$(R)* nginx exited$(W)"; \
# 	fi

# nginx-img:
# 	cd nginx && docker build -t $@ .

# nginx-rm:
# 	docker stop nginx;
# 	docker rm nginx;
# 	docker rmi nginx-img;

# G	=	\033[0;32m
# Y	=	\033[0;33m
# B	=	\033[0;34m
# R	=	\033[0;31m
# W	=	\033[0m
# N	=	\033[1;30m

# .PHONY: all clean fclean re nginx nginx-img nginx-rm
