/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:25:05 by fahmadia          #+#    #+#             */
/*   Updated: 2024/07/15 08:50:58 by ncasteln         ###   ########.fr       */
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
