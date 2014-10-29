#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

class HammingEncoder
{
public:
	string EncodeHamming(string message)
	{
		return CalculateHamming(message, true);
	}

	string DecodeHamming(string message)
	{
		return CalculateHamming(message, false);
	}

private:
	string CalculateHamming(string message, bool encode)
	{
		int numPBits;
		int numOnes = 0;
		string encodedMessage;
		int counter = 0;
		string testStr = "";

		int numSuccessful = 0;
		int numUnsuccessful = 0;
		int error = 0;

		numPBits = floor(log2(message.length())) + 1;

		if (encode)
		{
			for (int i = 0; i < numPBits; i++)
				message.insert(pow(2, i) - 1, "_");
		}

		encodedMessage = message;


		while (counter < numPBits)
		{
			testStr = determineTestString(counter, encodedMessage);

			if (encode)
				encodedMessage[pow(2, counter) - 1] = determineParity(testStr);
			else
			{
				if (encodedMessage[pow(2, counter) - 1] == determineParity(testStr.substr(1)))
				{
					//	cout << "check at " << pow(2, counter) - 1 << " successful" << endl;
					numSuccessful++;
				}
				else
				{
					cout << "parity check failure at position: " << pow(2, counter) - 1 << endl;
					numUnsuccessful++;
					error += pow(2, counter) - 1;
				}
			}

			testStr.clear();
			counter++;
		}

		if (!encode && numUnsuccessful != 0)
		{
			return "_" + to_string(error);
		}

		if (!encode && numUnsuccessful == 0)
		{
			for (int i = numPBits - 1; i >= 0; i--)
				encodedMessage.erase(pow(2, i) - 1, 1);
		}

		return encodedMessage;
	}

	string determineTestString(int counter, string encodedMessage)
	{
		string testStr = "";
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

		return testStr;

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