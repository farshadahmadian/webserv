/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:25:05 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/09 16:18:54 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __EXCEPTION_HPP__
# define __EXCEPTION_HPP__

# include <stdexcept>
# include <string>
# include "../../include/errors.h"

class Exception : public std::exception {
	public:
		Exception(void);
		Exception(std::string errMsg, enum server_err error);
		~Exception(void) throw();

		char const *what() const throw();

		enum server_err getError(void) const;
		void setError(enum server_err error);
	
	private:
		std::string _errMsg;
		enum server_err _error;
};

#endif /* __EXCEPTION_HPP__ */
