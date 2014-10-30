#include <iostream>
#include <bitset>
#include <time.h>

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

		numBytes = 3;
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
			string current;
			int randNum;
			srand(time(0));
			for (int i = 0; i < length; i++)
			{
				current = msg.substr(i * 12, 12);
				//rand()%(max-min)+min

				randNum = (rand() % (12));
				cout << randNum << endl;


			}
		}

		/*
		ALGORITHM

		deconstruct data string

		given number of bytes to change
		randomly select byte(s) to change
		-making sure not to change the same byte more than once
		-randomly select 1 bit of length of byte (12) to change, must be from 0 - 11 inclusive

		reconstruct string and return it
		*/
		return "";
	}
};