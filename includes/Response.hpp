/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: alkrusts <alkrusts@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/04 15:16:52 by alkrusts      #+#    #+#                 */
/*   Updated: 2022/08/04 18:02:38 by alkrusts      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "ReadyResponses.hpp"

class Response
{
	public:
		int				fd;
		std::string		header;
};

#endif
