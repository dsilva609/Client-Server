#include "Server.cpp"
#include "BinaryParser.cpp"
#include "CRCEncoder.cpp"
#include "HammingEncoder.cpp"

using namespace std;

int main()
{
	Server server;
	BinaryParser parser;
	vector<string> data;

	//data = parser.Parse("data.txt", false);

	//server.Start("127.0.0.1", "daytime", data);


	//	CRCEncoder encoder;
	//	string msg = encoder.Encode("110101");

	//	cout << encoder.Decode(msg) << endl;

	HammingEncoder hamming;

	cout << "encoded message: " << hamming.EncodeHamming("10011010") << endl;

	cout << hamming.DecodeHamming(hamming.EncodeHamming("10011010")) << endl;
	exit(EXIT_SUCCESS);
}