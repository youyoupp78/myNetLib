#include <cstdio>
#include "TcpServer.h"
#include "TcpClient.h"

int main(int argc, char ** argv)
{
	if(argv[1] == "1")
	{
		TcpServer server(5678);
	}
	else
	{
		TcpClient  client("localhost",5678);
	}
    return 0;
}