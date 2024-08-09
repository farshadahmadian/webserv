/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:12:05 by ncasteln          #+#    #+#             */
/*   Updated: 2024/08/09 12:33:53 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ERRORS_H__
# define __ERRORS_H__

enum file_err {
	E_TOOARGS = 110,
	E_INVFILE,
	E_ISDIR,
	E_FAIL
};

enum parse_err {
	E_ONLYTABS = 114,
	E_INVDIR,
	E_INVCONTEXT,
	E_INVIND,
	E_ENDDECL,
	E_EMPTYVAL,
	E_NOSERVER,
	E_NOLOCATION,
	E_CONTNAME,
	E_DUPLICATE
};

enum conf_err {
	CONF_SUCCESS = 0,
	E_TIMEOUT = 123,
	E_AUTOINDEX,
	E_CLIENTSIZE,
	E_HOST,
	E_PORT,
	E_METHOD,
};

enum server_err {
	NO_ERROR = 0,
	GET_ADDR_INFO_FAILD = 200,
	SOCKET_CREATION_FAILD,
	BIND_SOCKET_FAILD,
	LISTENING_FAILED,
	SOCKET_OPTIONS_FAILD,
	ACCEPTING_FAILED,
	POLL_FAILED,
	EVENT_ERROR,
	CLIENTSOCKET_RECEIVE_FAILED,
	CGI_FAIL,
	HTTP_REQUEST_FAILED //navid_code
};

#endif /* __ERRORS_H__ */
