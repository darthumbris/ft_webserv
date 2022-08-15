/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoogenb <shoogenb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 11:15:49 by shoogenb      #+#    #+#                 */
/*   Updated: 2022/08/15 13:46:30 by shoogenb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "Parse.hpp"
#include "Config.hpp"

int	main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "error, the arguments has to be length of 2\n";
		return 1;
	}
	try
	{
		std::ifstream file(argv[1]);
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
