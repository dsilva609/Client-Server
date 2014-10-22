#include "Server.cpp"
#include "BinaryParser.cpp"
#include "CRCEncoder.cpp"

using namespace std;

int main()
{
	Server server;
	BinaryParser parser;
	vector<string> data;

	//data = parser.Parse("data.txt", false);

	//server.Start("127.0.0.1", "daytime", data);


	CRCEncoder encoder;

	encoder.Calculate("");


	exit(EXIT_SUCCESS);
}