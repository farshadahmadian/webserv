/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 14:33:17 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/16 17:18:04 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

typedef enum e_error {
	NO_ERROR = 0,
	GET_ADDR_INFO_FAILD = 200,
	SOCKET_CREATION_FAILD,
	BIND_SOCKET_FAILD,
	LISTENING_FAILED,
	SOCKET_OPTIONS_FAILD,
	ACCEPTING_FAILED,
} t_error;

#endif