/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoogenb <shoogenb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 11:15:49 by shoogenb      #+#    #+#                 */
/*   Updated: 2022/07/04 12:18:37 by shoogenb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

int	main(void)
{
	char	*srv_address;

	srv_address = strdup("127.0.0.1");
	WebServ	server_one(8080, srv_address);
	while (true)
	{
		server_one.runServer();
	}
	return (EXIT_SUCCESS);
}
