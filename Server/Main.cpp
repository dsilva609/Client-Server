#include "Server.cpp"
#include "BinaryParser.cpp"
#include "HDB3Parser.cpp"

using namespace std;

int main()
{
	Server server;
	BinaryParser parser;
	HDB3Parser hdb3Parser;
	vector<string> data;

	data = parser.Parse("data.txt", false);

	data = hdb3Parser.Encode(data);

	server.Start("127.0.0.1", "44300", data);

	exit(EXIT_SUCCESS);
}