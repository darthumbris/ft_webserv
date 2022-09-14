/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoogenb <shoogenb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 11:15:49 by shoogenb      #+#    #+#                 */
/*   Updated: 2022/09/02 13:35:05 by shoogenb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "Parse.hpp"
#include "Config.hpp"

//TODO cleanup the var/www/html files

int	main(int argc, char *argv[])
{
	if (argc > 2)
	{
		std::cerr << "error, only 0 or 1 argument accepted." << std::endl;
		return 1;
	}
	Json *json;
	try
	{
		std::string path;
		if (argc != 2)
			path = "configs/default.json";
		else
			path = argv[1];
		std::ifstream file(path);
		if (file.fail())
		{
			std::cerr << "Error: could not open file." << std::endl;
			exit(1);
		}
		json = Parse().parse(file);
		file.close();

	} 
	catch (std::exception const &e)
	{
		system("leaks webserv");
		std::cerr << e.what() << std::endl;
		std::cout << "Exiting webserv." << std::endl;
		return 1;
	}
	try
	{
		Config config(json);
		delete json;
		t_servmap	servers = config.getServerMap();
		WebServ	webserver(servers);
		webserver.runServer();
	}
	catch (std::exception const &e)
	{
		delete json;
		system("leaks webserv");
		std::cerr << e.what() << std::endl;
		std::cout << "Exiting webserv." << std::endl;
		return 1;
	}
	return (EXIT_SUCCESS);
}
