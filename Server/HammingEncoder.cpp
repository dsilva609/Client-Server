#include <iostream>
#include <string>

using namespace std;

class HammingEncoder
{
public:
	string Encode(string message)
	{
		CalculateHamming(message);

		return "";
	}

	string Decode(string message)
	{
		return "";
	}

private:
	string CalculateHamming(string message)
	{
		int numPBits;
		int numOnes = 0;
		string encodedMessage;

		numPBits = floor(log2(message.length())) + 1;
		cout << numPBits << endl;

		for (int i = 1; i <= message.length(); i += 2)
		{
			if (message[i] == '1')
				numOnes++;
		}

		if (isOdd(numOnes))
			encodedMessage = "0" + message;
		else
			encodedMessage = "1" + message;

		cout << "message: " << encodedMessage << endl;

		return "";
	}

	bool isOdd(int num)
	{
		if (num % 2 == 1)
			return true;
		return false;
	}
};
