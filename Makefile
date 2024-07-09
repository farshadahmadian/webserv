# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/12 10:15:37 by ncasteln          #+#    #+#              #
#    Updated: 2024/07/09 16:54:02 by nnabaeei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #





NAME     := mutantstack
CXX      := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -g
CXXFLAGS += -fsanitize=address -Wshadow -Wno-shadow

# Directories
SRC_DIR := src
OBJ_DIR := obj

# Find all .cpp files in the SRC directory and subdirectories
CPP_FILES := $(shell find $(SRC_DIR) -name '*.cpp')

# Find all .hpp files in the SRC directory and subdirectories
HEADERS := $(shell find $(SRC_DIR) -name '*.hpp')

# Generate object file names, preserving directory structure
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_FILES))

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	@$(CXX) -c -o $@ $< $(CXXFLAGS) -I$(SRC_DIR)

$(NAME): $(OBJ_FILES)
	@$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(NAME) $(LDFLAGS)
	@echo "$(ORG)----- $(RESET)$(GREEN)$(NAME)$(RESET)$(ORG) as exe file is created!-----$(RESET)"

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

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


.PHONY: all clean fclean re

BLUE    = \033[38;5;4m
GREEN   = \033[38;5;2m
ORG     = \033[38;5;214m
RED     = \033[38;5;196m
RESET   = \033[0m

