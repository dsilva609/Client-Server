#include "Server.cpp"
#include "BinaryParser.cpp"
#include "ErrorGenerator.cpp"

using namespace std;

int main()
{
	Server server;
	BinaryParser parser;
	vector<string> data;
	ErrorGenerator generator;

	data = parser.Parse("data.txt", false);
	data = generator.Execute(data, 5);

	server.Start("127.0.0.1", "daytime", data);

	exit(EXIT_SUCCESS);
}