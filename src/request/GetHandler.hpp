/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:36:51 by nnabaeei          #+#    #+#             */
/*   Updated: 2024/07/29 22:47:55 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETHANDLER_HPP
# define  GETHANDLER_HPP

# include "./HttpResponse.hpp"

class GetHandler: public HTTPResponse {
	private:
	public:
		GetHandler();
		~GetHandler();
		GetMethod();
};

#endif
