#include <iostream>
#include <bitset>

using namespace std;

class ErrorGenerator
{
public:
	string GeneratorError(int numBytes, string message)
	{
		string controlLengthStr;
		int length;
		string crcHash;
		string msg;

		numBytes = 2;
		message = "0001011000010110000000110000110000011101110000100010110100111010101110001000";


		controlLengthStr = message.substr(0, 24);
		length = bitset<8>(message.substr(16, 8)).to_ulong();
		crcHash = message.substr(message.length() - 16, 16);
		msg = message.substr(24, message.length() - (controlLengthStr.length() + crcHash.length()));

		if (numBytes > length)
		{
			//only change bits for length amount of bytes
		}
		else
		{

		}


		return "";
	}
private:
};