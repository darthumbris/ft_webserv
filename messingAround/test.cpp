#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <iostream>

int	main()
{
	int	fd;

	fd = kqueue();
	if (fd == -1)
		std::cout << "kque failed!" << std::endl;
	return (0);
}
