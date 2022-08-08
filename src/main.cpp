/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoogenb <shoogenb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 11:15:49 by shoogenb      #+#    #+#                 */
/*   Updated: 2022/08/08 16:24:08 by shoogenb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "Config.hpp"
#include "CgiHandler.hpp"

int	main(int argc, char *argv[])
{
	Config				*config;

	if (argc == 2)
		config = new Config(argv[1]);
	else
		config = new Config("default.conf");

	//Testing cgi stuff
	std::cout << "test of php-cgi\n" << std::endl;
	CgiHandler test;
	std::cout << test.execute() << std::endl << std::endl;

	WebServ	webserver(config);
	webserver.runServer();
	return (EXIT_SUCCESS);
}
