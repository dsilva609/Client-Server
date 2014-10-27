#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bitset>
#include <boost/foreach.hpp>
#include "CRCEncoder.cpp"
#include "HammingEncoder.cpp"

using namespace boost;
using namespace std;

#define SYN char(22)

class BinaryParser
{
public:
	vector<string> Parse(string filename, bool read)
	{
		Read(filename);
		if (read)
			DecodeData(this->_data);
		else
			EncodeData();

		return this->_encodedData;
	}

private:
	vector<string> _data;
	vector<string> _encodedData;
	CRCEncoder _crcEncoder;//apply to entire frame
	HammingEncoder _hammingEncoder;//apply only to message

	void Read(string filename)
	{
		fstream stream;
		string lineIn;

		stream.open(filename);

		if (stream.good())
		{
			cout << "Reading contents of " << filename << "..." << endl;
			while (stream.good())
			{
				getline(stream, lineIn);

				this->_data.push_back(lineIn);
			}
			stream.close();
		}
		else
			cout << "ERROR: Could not open file: " << filename << endl;
	}

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

	void EncodeData()
	{
		string temp;
		temp = "";

		for each (auto str in this->_data)
		{
			temp += ConvertToBinary(to_string(SYN), false, true);
			temp += ConvertToBinary(to_string(SYN), false, true);

			temp += ConvertToBinary(to_string(str.length()), false, true);//string is SYN SYN length
			str = ConvertToBinary(str, true);//message string, do hamming encoding

			//cout << "str before hamming:" << endl << str << endl;

			str = EncodeBytesToHamming(str);// this->_hammingEncoder.EncodeHamming(str);

			//cout << "str after hamming:" << endl << str << endl;
			temp += str;

			// cout << "temp before crc encode: " << endl << temp << endl;

			temp += this->_crcEncoder.EncodeCRC(temp);
			//cout << "temp after crc encode: " << endl << temp << endl;

			this->_encodedData.push_back(temp);
			temp.clear();
		}
	}

	string EncodeBytesToHamming(string message)
	{
		string encodedHamming = "";

		for (int i = 0; i < message.length() / 8; i++)
		{
			//	cout << "byte: " << message.substr(i, 8) << endl;
			encodedHamming += this->_hammingEncoder.EncodeHamming(message.substr(i, 8));
			//	cout << "hamm: " << this->_hammingEncoder.EncodeHamming(message.substr(i, 8)) << endl;
			//	cout << "encoded hamming now: " << encodedHamming << endl;
		}
		cout << "encoded hamming now: " << encodedHamming << endl;

		return encodedHamming;
	}

	void DecodeData(vector<string> data)
	{
		string syn1;
		string syn2;
		int length;
		string message;
		string temp = "";
		int pos = 0;
		char bit;

		cout << "Contents: " << endl;
		for each(auto item in data)
		{
			if (item.size() == 0)
				continue;

			syn1 = item.substr(0, 8);
			syn2 = item.substr(8, 16);

			length = bitset<8>(item.substr(16, 24)).to_ulong();
			if (length == 0)
				continue;

			message = item.substr(24);

			cout << "\t";
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
		}
	}
};