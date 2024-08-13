/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 12:03:53 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/13 11:43:00 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSER_H__
# define __PARSER_H__

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sys/stat.h>	// stat()
#include "../../include/errors.h"
#include "colors.h"

#define DEFAULT_FILE_PATH		"./conf/default.conf"

/* NUMBER OF DIRECTIVES */
#define N_SHARED_DIR	5
#define N_SERVER_DIR	3
#define N_LOCATION_DIR	3

/* SHARED DIRECTIVE DEFAULTS */
#define DEFAULT_TIMEOUT			"10"
#define DEFAULT_CLIENT_SIZE		"80"
#define DEFAULT_INDEX			"index index.html"
#define DEFAULT_AUTOINDEX		"on"
#define DEFAULT_ROOT			"./www/"

/* SERVER DIRECTIVE DEFAULTS */
#define DEFAULT_SERVER_NAME		"127.0.0.1" 	/* need to decide something */
#define DEFAULT_PORT			"8080"

/* LOCATION DIRECTIVE DEFAULTS */
#define DEFAULT_URI				"/"
#define DEFAULT_METHOD			"GET"
#define DEFAULT_CGI				"" 				/* need to decide something */

#define	SPACES				" \t\v\f\r"
#define	COMMENT(c)			((c) == '#')
#define	ENDVALUE(c)			((c) == ';')

enum context {
	INIT = -1,
	HTTP,
	SERVER,
	LOCATION,
	COMMENT
};

#endif /* __PARSER_H__ */
