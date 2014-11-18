#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bitset>
#include <boost/foreach.hpp>

using namespace std;
using namespace boost;

#define SYN char(22)

class BinaryParser
{
public:
	//public parser entry point for encode and decode
	vector<string> Parse(string filename, bool read)
	{
		Read(filename, read);

		if (read)
			DecodeData(this->_data);
		else
			EncodeData();

		return this->_encodedData;
	}

private:
	vector<string> _data;
	vector<string> _encodedData;

	//reads in content of given file
	void Read(string filename, bool read)
	{
		fstream stream;
		string lineIn;
		string temp;

		stream.open(filename);

		if (stream.good())
		{
			cout << "Reading contents of " << filename << "..." << endl;
			while (stream.good())
			{
				getline(stream, lineIn);

				if (!read && lineIn.length() > 64)
				{
					while (lineIn.length() > 64)
					{
						temp = lineIn.substr(0, 64);
						this->_data.push_back(temp);
						lineIn = lineIn.substr(64);
					}
					this->_data.push_back(lineIn);
				}
				else
					this->_data.push_back(lineIn);
			}
			stream.close();
		}
		else
			cout << "ERROR: Could not open file: " << filename << endl;
	}

	//converts given string to binary with options of parity or decimal conversion
	string ConvertToBinary(string str, bool withParity, bool isInt = false)
	{
		string temp = "";
		string binStr = "";
		char bit;
		if (isInt)
			return bitset<8>(stoi(str)).to_string();

		if (withParity)
		{
			for (int i = 0; i < str.length(); i++)
			{
				temp += bitset<7>(str[i]).to_string();

				bit = EvaluateParity(temp);
				binStr += bit + temp;
				temp.clear();
			}
			temp = binStr;
		}
		else
		{
			for (int i = 0; i < str.length(); i++)
				temp += bitset<8>(str[i]).to_string();
		}
		return temp;
	}

	//determines odd parity for given binary string
	char EvaluateParity(string binStr)
	{
		int numOnes = 0;

		for each (auto elem in binStr)
		{
			if (elem == '1')
				numOnes++;
		}

		if (numOnes % 2 == 0)
			return '1';

		return '0';
	}

	//encodes SYN, SYN, Length, and Message with hamming and CRC hash
	void EncodeData()
	{
		string temp;
		temp = "";

		for each (auto str in this->_data)
		{
			temp += ConvertToBinary(to_string(SYN), false, true);
			temp += ConvertToBinary(to_string(SYN), false, true);

			temp += ConvertToBinary(to_string(str.length()), false, true);

			str = ConvertToBinary(str, true);

			temp += str;

			this->_encodedData.push_back(temp);
			temp.clear();
		}
	}

	//decodes data back to original ASCII representation and determines which strings fail the CRC check
	void DecodeData(vector<string> data)
	{
		string syn1;
		string syn2;
		int length;
		string message;
		string temp = "";
		int pos = 0;
		char bit;
		int count = 0;

		cout << "Contents: " << endl;
		for each(auto item in data)
		{
			if (item.size() == 0)
			{
				count++;
				continue;
			}

			syn1 = item.substr(0, 8);
			syn2 = item.substr(8, 8);

			length = bitset<8>(item.substr(16, 8)).to_ulong();
			if (length == 0)
			{
				count++;
				continue;
			}

			message = item.substr(24);

			while ((pos / 8) < length)
			{
				for (int i = pos; i < pos + 8; i++)
					temp += message.at(i);

				pos = pos + 8;
				bit = EvaluateParity(temp);
				if (bit == '0')
				{
					temp = char(bitset<7>(temp.substr(1)).to_ulong());

					cout << temp;
				}
				else
					cout << "Parity check failed.";
				temp.clear();
			}
			pos = 0;
			cout << endl;
			count++;
		}
	}
};