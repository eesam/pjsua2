#include <stdio.h>
#include <iostream>
#include "pjsua2_server.h"

#include <pj++/lock.hpp>
#include <pj++/os.hpp>
#include <pj++/sock.hpp>


int main( int argc, char* argv[] )
{
	Pjsua2_Server server;
	server.start();

	std::cin.get();
	server.stop();
	return 0;
}
