/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoogenb <shoogenb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 11:15:49 by shoogenb      #+#    #+#                 */
/*   Updated: 2022/08/09 11:16:01 by shoogenb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

int	main(int argc, char *argv[])
{
	Config				*config;

	if (argc == 2)
		config = new Config(argv[1]);
	else
		config = new Config("default.conf");
	WebServ	webserver(config);
	webserver.runServer();
	return (EXIT_SUCCESS);
}
