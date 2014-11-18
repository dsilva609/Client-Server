#include "Server.cpp"
#include "BinaryParser.cpp"

using namespace std;

int main()
{
	Server server;
	BinaryParser parser;
	vector<string> data;

	data = parser.Parse("data.txt", false);

	server.Start("127.0.0.1", "44300", data);

	exit(EXIT_SUCCESS);
}