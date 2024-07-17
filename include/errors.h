/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:12:05 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/17 11:09:29 by nnabaeei         ###   ########.fr       */
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
	E_CONTNAME
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
	HTTP_REQUEST_FAILED //navid_code
};

#endif /* __ERRORS_H__ */
