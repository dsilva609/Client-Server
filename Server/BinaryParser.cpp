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

struct CorruptFrame
{
	string frameData;
	int frameNum;
};

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
	CRCEncoder _crcEncoder;
	HammingEncoder _hammingEncoder;

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

			temp += ConvertToBinary(to_string(str.length()), false, true);

			str = ConvertToBinary(str, true);
			str = EncodeBytesToHamming(str);

			temp += str;
			temp = this->_crcEncoder.EncodeCRC(temp);

			this->_encodedData.push_back(temp);
			temp.clear();
		}
	}

	string EncodeBytesToHamming(string message)
	{
		string encodedHamming = "";

		for (int i = 0; i < message.length(); i += 8)
			encodedHamming += this->_hammingEncoder.EncodeHamming(message.substr(i, 8));

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
		string result;
		int count = 0;

		int numSuccessful = 0;
		int numUnsuccessful = 0;
		CorruptFrame corruptData;
		vector<CorruptFrame> corruptFrames;

		cout << "Contents: " << endl;
		for each(auto item in data)
		{
			if (item.size() == 0)
			{
				count++;
				continue;
			}
			result = this->_crcEncoder.DecodeCRC(item);

			if (result.length() != 0)
			{
				syn1 = result.substr(0, 8);
				syn2 = result.substr(8, 8);

				length = bitset<8>(result.substr(16, 8)).to_ulong();
				if (length == 0)
				{
					count++;
					continue;
				}

				message = result.substr(24);

				item = DecodeBytesFromHamming(message);
				message = item;
			}
			else
			{
				cerr << "CRC check failure at frame: " << count << endl;

				corruptData.frameData = item;
				corruptData.frameNum = count;
				corruptFrames.push_back(corruptData);

				count++;
				numUnsuccessful++;

				continue;
			}

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
			numSuccessful++;
			count++;
		}

		cout << "successful decodes: " << numSuccessful << endl;
		cout << "unsuccessful decodes: " << numUnsuccessful << endl;

		if (numUnsuccessful != 0)
			CorrectFrame(corruptFrames);
	}

	string DecodeBytesFromHamming(string message)
	{
		string decodedHamming = "";

		for (int i = 0; i < message.length(); i += 12)
			decodedHamming += this->_hammingEncoder.DecodeHamming(message.substr(i, 12));

		return decodedHamming;
	}

	void CorrectFrame(vector<CorruptFrame> corruptData)
	{
		vector<string> correctedData;

		cout << endl << "Performing error correction..." << endl;

		for each(auto item in corruptData)
		{
			string temp;
			string errorLocations = "";

			string controlLengthStr;
			string crcHash;
			string message;
			string correctedMessage = "";

			cout << "Correcting frame: " << item.frameNum << endl;

			crcHash = item.frameData.substr(item.frameData.length() - 16, 16);
			item.frameData = item.frameData.substr(0, item.frameData.length() - 16);//remove crc
			controlLengthStr = item.frameData.substr(0, 24);//save syn syn length

			message = item.frameData.substr(24);//message

			for (int i = 0; i < message.length(); i += 12)
			{
				string currentStr;
				currentStr = message.substr(i, 12);
				cout << "checking byte: " << (i / 12) + 1 << endl;
				temp = DecodeBytesFromHamming(currentStr);

				if (temp[0] == '_')
				{
					errorLocations = (temp.substr(1));

					if (currentStr[stoi(errorLocations) - 1] == '1')
						currentStr[stoi(errorLocations) - 1] = '0';
					else
						currentStr[stoi(errorLocations) - 1] = '1';

				}
				correctedMessage += currentStr;
				errorLocations.clear();
				temp.clear();
			}

			correctedData.push_back(controlLengthStr + correctedMessage + crcHash);
		}

		cout << endl << "Decoding corrected data..." << endl;
		DecodeData(correctedData);
	}
};