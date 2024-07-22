/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:37:04 by ncasteln          #+#    #+#             */
/*   Updated: 2024/07/15 11:42:15 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _COLORS_H_
# define _COLORS_H_

#define RESET	"\x1B[0m"
#define RED		"\x1B[31m"
#define GREEN	"\x1B[32m"
#define YELLOW	"\x1B[33m"
#define BLUE	"\x1B[34m"
#define PURPLE	"\x1B[35m"
#define MAGENTA	"\033[38;5;5m"
#define ORG		"\033[38;5;214m"
#define CYAN		"\033[38;5;44m"

#define R(x)	RED x RESET
#define G(x)	GREEN x RESET
#define Y(x)	YELLOW x RESET
#define B(x)	BLUE x RESET
#define P(x)	PURPLE x RESET

#define BOLD(x)	"\x1B[1m" x RESET
#define UNDL(x)	"\x1B[s4m" x RESET

#endif /* _COLORS_H_ */
// # define BLUE		"\033[38;5;4m"
// # define GREEN		"\033[38;5;2m"
// # define RED		"\033[38;5;196m"
// # define RESET		"\033[0m"