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

	generator.GeneratorError(0, "");


	//	data = parser.Parse("data.txt", false);

	//server.Start("127.0.0.1", "daytime", data);


	//CRCEncoder encoder;
	//string msg = encoder.EncodeCRC("00010110000101100000010011110100111001010111001111110100");
	//cout << encoder.DecodeCRC(msg) << endl;

	//HammingEncoder hamming;

	//cout << "encoded message: " << hamming.EncodeHamming("10011010") << endl;

	//cout << hamming.DecodeHamming(hamming.EncodeHamming("10011010")) << endl;

	exit(EXIT_SUCCESS);
}