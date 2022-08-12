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

	if (argc > 2)
	{
		std::cout << "argument to long" << std::endl;
		return 1;
	}
	else if (argc == 2)
		config = new Config(argv[1]);
	else
<<<<<<< HEAD
		config = new Config("default.conf");
	WebServ	webserver(config);
	webserver.runServer();
=======
		config = new Config("config/default.conf");
	// srv_address = strdup("127.0.0.1");
	// WebServ	server_one(8080, srv_address);
//	WebServ	webserver(config);
//	webserver.runServer();
>>>>>>> 1cbafb13e20a6e09f6da15a449c106575de5405e
	return (EXIT_SUCCESS);
}
