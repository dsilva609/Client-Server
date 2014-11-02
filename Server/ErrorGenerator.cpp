#include <iostream>
#include <bitset>
#include <time.h>
#include <vector>

using namespace std;

class ErrorGenerator
{
public:
	//performs error generation routines
	vector<string> Execute(vector<string> data, int numFrames)
	{
		int genErrorFrameStartPos;

		srand(time(0));

		genErrorFrameStartPos = DetermineFrameErrorStartPosition(numFrames, data.size());
		if (genErrorFrameStartPos == -1)
		{
			numFrames = data.size();
			genErrorFrameStartPos = 0;
		}

		for (int i = genErrorFrameStartPos; i < genErrorFrameStartPos + numFrames; i++)
			data[i] = GenerateError(data[i]);

		return data;
	}

private:
	//determines position where first error will begin
	int DetermineFrameErrorStartPosition(int numFrames, int numDataElements)
	{
		int pos;

		if (numFrames == numDataElements)
			pos = 0;
		else if (numFrames > numDataElements)
			pos = -1;
		else
			pos = rand() % (numDataElements - numFrames + 1);

		return pos;
	}

	//generates error for a random number of bytes for a given string
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

		for (int i = 0; i < randNumBytes; i++)
		{
			current = msg.substr(i * 12, 12);
			randBitPos = (rand() % 12 + 1) - 1;

			if (msg[i * 12 + randBitPos] == '1')
				msg[i * 12 + randBitPos] = '0';
			else
				msg[i * 12 + randBitPos] = '1';
		}

		return controlLengthStr + msg + crcHash;
	}
};