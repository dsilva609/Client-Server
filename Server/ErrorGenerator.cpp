#include <iostream>
#include <bitset>
#include <time.h>
#include <vector>

using namespace std;

class ErrorGenerator
{
public:
	vector<string> Execute(vector<string> data, int numFrames)
	{

		int genErrorFrameStartPos;

		srand(time(0));

		//remove these three
		//	numFrames = 9;

		genErrorFrameStartPos = DetermineFrameErrorStartPosition(numFrames, data.size());
		if (genErrorFrameStartPos == -1)
		{
			numFrames = data.size();
			genErrorFrameStartPos = 0;
		}
		//cout << genErrorFrameStartPos << endl;
		//string message;
		//message = "0001011000010110000000110000110000011101110000100010110100111010101110001000";

		for (int i = genErrorFrameStartPos; i < genErrorFrameStartPos + numFrames; i++)
		{
			data[i] = GenerateError(data[i]);
		}


		//controlLengthStr = message.substr(0, 24);
		//length = bitset<8>(message.substr(16, 8)).to_ulong();
		//crcHash = message.substr(message.length() - 16, 16);
		//msg = message.substr(24, message.length() - (controlLengthStr.length() + crcHash.length()));



		//randNumBytes = rand() % length + 1;

		//cout << "number of bytes: " << randNumBytes << endl;
		//for (int i = 0; i < randNumBytes; i++)
		//{
		//	current = msg.substr(i * 12, 12);
		//	randBitPos = (rand() % 12 + 1) - 1;

		//	if (msg[i * 12 + randBitPos] == '1')
		//		msg[i * 12 + randBitPos] = '0';
		//	else
		//		msg[i * 12 + randBitPos] = '1';

		//	cout << "current is: " << endl << current << endl;
		//	cout << "byte is now: " << endl << msg.substr(i * 12, 12) << endl;

		//	//cout << randBitPos << endl;
		//	//	//rand()%(max-min)+min
		//}

		//return controlLengthStr + msg + crcHash;
		return data;

		/*
		ALGORITHM

		deconstruct data string

		given number of bytes to change
		-make sure that number of bytes to change is less than or equal to number of bytes in message

		while count is less than number of bytes to change
		-randomly select byte(s) to change
		-making sure not to change the same byte more than once
		-randomly select 1 bit of length of byte (12) to change, must be from 0 - 11 inclusive

		reconstruct string and return it
		*/
	}

private:
	int DetermineFrameErrorStartPosition(int numFrames, int numDataElements)
	{
		int pos;

		if (numFrames == numDataElements)
			pos = 0;
		else if (numFrames > numDataElements)
			pos = -1;
		else
		{
			pos = rand() % (numDataElements - numFrames + 1);
		}
		return pos;
	}

	string GenerateError(string message)
	{
		string controlLengthStr;
		int length;
		string crcHash;
		string msg;
		string current;
		int randNumBytes;
		int randBitPos;

		controlLengthStr = message.substr(0, 24);
		length = bitset<8>(message.substr(16, 8)).to_ulong();
		crcHash = message.substr(message.length() - 16, 16);
		msg = message.substr(24, message.length() - (controlLengthStr.length() + crcHash.length()));



		randNumBytes = rand() % length + 1;

		//	cout << "number of bytes: " << randNumBytes << endl;
		for (int i = 0; i < randNumBytes; i++)
		{
			current = msg.substr(i * 12, 12);
			randBitPos = (rand() % 12 + 1) - 1;

			if (msg[i * 12 + randBitPos] == '1')
				msg[i * 12 + randBitPos] = '0';
			else
				msg[i * 12 + randBitPos] = '1';

			//cout << "current is: " << endl << current << endl;
			//		cout << "byte is now: " << endl << msg.substr(i * 12, 12) << endl;

			//cout << randBitPos << endl;
			//	//rand()%(max-min)+min
		}

		return controlLengthStr + msg + crcHash;
	}
};