/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:12:05 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/20 15:12:45 by ncasteln         ###   ########.fr       */
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


#endif /* __ERRORS_H__ */
