#include "../includes/parse.hpp"
#include "../includes/Config.hpp"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "error" << std::endl;
		return 1;
	}
	try {
		std::ifstream file(argv[1]);
		Parse parser;
		Json* json = parser.parse(file);
		Config config(json);
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
