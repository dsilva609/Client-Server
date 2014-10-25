#include <iostream>
#include <string>

using namespace std;

class HammingEncoder
{
public:
	string EncodeHamming(string message)
	{
		return CalculateHamming(message);
	}

	string DecodeHamming(string message)
	{
		return "";
	}

private:
	string CalculateHamming(string message)
	{
		int numPBits;
		int numOnes = 0;
		string encodedMessage;
		int counter = 0;
		string testStr = "";

		numPBits = floor(log2(message.length())) + 1;

		for (int i = 0; i < numPBits; i++)
			message.insert(pow(2, i) - 1, "_");

		encodedMessage = message;


		while (counter < numPBits)
		{
			//		cout << "counter is: " << counter << endl;
			for (int i = pow(2, counter) - 1; i < encodedMessage.length(); i += pow(2, counter) * 2)
			{
				//	cout << "i is: " << i << endl;

				if (i + pow(2, counter) > encodedMessage.length())
				{
					//cout << "checking: " << encodedMessage.substr(i) << endl;
					testStr += encodedMessage.substr(i);
				}
				else
				{
					//	cout << "checking: " << encodedMessage.substr(i, pow(2, counter)) << endl;
					testStr += encodedMessage.substr(i, pow(2, counter));
				}
			}

			encodedMessage[pow(2, counter) - 1] = determineParity(testStr);

			testStr.clear();
			counter++;
		}

		//cout << "encoded message: " << encodedMessage << endl;

		return encodedMessage;
	}

	char determineParity(string testStr)
	{
		int num = 0;

		for (int i = 0; i < testStr.length(); i++)
		{
			if (testStr[i] == '1')
				num++;
		}

		if (isOdd(num))
			return '0';

		return '1';

	}

	bool isOdd(int num)
	{
		if (num % 2 == 1)
			return true;
		return false;
	}
};
