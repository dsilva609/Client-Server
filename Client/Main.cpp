#include "Client.cpp"
#include "BinaryParser.cpp"

using namespace std;

int main()
{
	Client client;

	client.Start("127.0.0.1", "TransmittedData.txt");

	BinaryParser parser;

	parser.Parse("TransmittedData.txt", true);

	exit(EXIT_SUCCESS);
}