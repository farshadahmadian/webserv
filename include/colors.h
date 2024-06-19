/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:37:04 by ncasteln          #+#    #+#             */
/*   Updated: 2024/06/15 11:27:23 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _COLORS_H_
# define _COLORS_H_

#define RESET	"\x1B[0m"
#define RED		"\x1B[31m"
#define GREEN	"\x1B[32m"
#define YELLOW	"\x1B[33m"
#define BLUE	"\x1B[34m"

#define R(x)	RED x RESET
#define G(x)	GREEN x RESET
#define Y(x)	YELLOW x RESET
#define B(x)	BLUE x RESET

#define BOLD(x)	"\x1B[1m" x RESET
#define UNDL(x)	"\x1B[s4m" x RESET

#endif /* _COLORS_H_ */
