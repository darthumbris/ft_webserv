/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoogenb <shoogenb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 11:15:49 by shoogenb      #+#    #+#                 */
/*   Updated: 2022/08/15 14:20:04 by shoogenb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "Parse.hpp"
#include "Config.hpp"

int	main(int argc, char *argv[])
{
	if (argc > 2)
	{
		std::cout << "error, only 0 or 1 argument accepted." << std::endl;
		return 1;
	}
	try
	{
		std::string path;
		if (argc != 2)
			path = "configs/default.json";
		else
			path = argv[1];
		std::ifstream file(path);
		Parse parse;
		Json* json = parse.parse(file);
		Config config(json);
		WebServ	webserver(&config);
		webserver.runServer();
	} 
	catch (std::exception const &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return (EXIT_SUCCESS);
}
