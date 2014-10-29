#include <iostream>
#include <bitset>
#include <string>

using namespace std;

#define CRCANSI "11000000000000101"

class CRCEncoder
{
public:
	string EncodeCRC(string message)
	{
		string hash;
		string encodedMessage;
		int size;

		encodedMessage = message;
		message += this->_padding;
		hash = Calculate(message);
		size = 16 - hash.length();

		for (int i = 0; i < size; i++)
			hash = "0" + hash;

		encodedMessage += hash;

		return encodedMessage;
	}

	string DecodeCRC(string message)
	{
		string decodedMessage;
		string result;

		result = Calculate(message);

		if (bitset<8>(result) == 0)
			decodedMessage = message.substr(0, message.length() - 16);
		else
			return "";
		//	cerr << "CRC check failure!" << endl;

		return decodedMessage;
	}

private:
	string _padding = "0000000000000000";
	string _divisor = CRCANSI;

	string Calculate(string message)
	{
		string result = "";

		string current;
		string next;

		int i = 0;
		while ((message.length() >= this->_divisor.length()))
		{
			result.clear();

			current = message.substr(0, this->_divisor.length());

			if (current[i] == '0')
			{
				i++;
				continue;
			}

			//cout << "current dividend: " << current << endl;


			for (int j = 0; j < this->_divisor.length(); j++)
			{
				if ((current[j] == '1' && this->_divisor[j] == '1') || (current[j] == '0'&& this->_divisor[j] == '0'))
					result += '0';
				else
					result += '1';
			}

			next = result + message.substr(this->_divisor.size());

			//	cout << "result: " << result << endl;

			for (int k = 0; k < next.length(); k++)
			{
				if (next[k] == '1')
				{
					next = next.substr(k);
					break;
				}
			}
			//	cout << "current now: " << next << endl;

			message = next;


			if (message.length() < this->_divisor.length())
			{
				result = message;
				break;

			}

			if (BinaryToDecimal(message) == 0)
			{
				result = "0";
				break;
			}
		}

		//cout << "final result: " << result << endl;

		return result;
		/*   ALGORITHM
		while length of divisor < number

		xor by divisor

		shift to right by 1
		if length is less than divisor starting with first 1
		shift to right again

		*/
	}

	int BinaryToDecimal(string binary)
	{
		int num = 0;

		for (int i = binary.length(); i >= 0; i--)
		{
			if (binary[i] == '1')
				num += pow(2, binary.length() - i);
		}

		return num;
	}
};