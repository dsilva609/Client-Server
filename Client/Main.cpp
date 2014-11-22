#include "Client.cpp"
#include "BinaryParser.cpp"
#include "HDB3Parser.cpp"

using namespace std;

int main()
{
	Client client;
	BinaryParser parser;
	HDB3Parser hdb3Parser;
	vector<string> data;

	client.Start("127.0.0.1", "TransmittedData.txt");
	data = hdb3Parser.Decode("TransmittedData.txt", "ConvertedFromHDB3.txt");
	data = parser.Parse("ConvertedFromHDB3.txt", true);



	exit(EXIT_SUCCESS);
}