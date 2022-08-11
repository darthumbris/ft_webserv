#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

int	main()
{
	std::ifstream ifs;

	ifs.open("test.txt", std::ifstream::in);
	int	p = access("test.txt", F_OK);
	std::cout << "FILE EXISTS: " << p << std::endl;
	p = access("test.txt", R_OK);
	std::cout << "FILE IS REDABLE: " << p << std::endl;
	p = access("test.txt", W_OK);
	std::cout << "FILE IS WRITABLE: " << p << std::endl;

	std::cout << "FAILED: " << ifs.fail() << std::endl;

	char c = ifs.get();

	while (ifs.good()) {
		std::cout << c;
		c = ifs.get();
	}

	ifs.close();
	int	status = remove("dada");
	std::cout << "remove status: " << status << std::endl;

	return 0;	
}
