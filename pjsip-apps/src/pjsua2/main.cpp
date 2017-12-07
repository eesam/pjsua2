#include <stdio.h>
#include <iostream>
#include "pjsua2_server.h"

int main( int argc, char* argv[] )
{
	getServerInstance()->start();
	std::cin.get();
	getServerInstance()->stop();
	return 0;
}
