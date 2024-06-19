/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahmadia <fahmadia@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:25:05 by fahmadia          #+#    #+#             */
/*   Updated: 2024/06/19 14:23:07 by fahmadia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <stdexcept>
# include <string>
# include <error.hpp>

class Exception : public std::exception {
	public:
		Exception(void);
		Exception(std::string errMsg, t_error error);
		~Exception(void) throw();

		char const *what() const throw();

		t_error getError(void) const;
		void setError(t_error error);
	
	private:
		std::string _errMsg;
		t_error _error;
};

#endif
