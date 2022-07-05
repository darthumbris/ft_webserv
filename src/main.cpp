/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoogenb <shoogenb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 11:15:49 by shoogenb      #+#    #+#                 */
/*   Updated: 2022/07/05 14:04:59 by shoogenb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "Config.hpp"

int	main(int argc, char *argv[])
{
	Config	*config;

	if (argc == 2)
		config = new Config(argv[1]);
	else
		config = new Config("default.conf");
	// srv_address = strdup("127.0.0.1");
	// WebServ	server_one(8080, srv_address);
	WebServ	webserver(config);
	webserver.runServer();
	return (EXIT_SUCCESS);
}
