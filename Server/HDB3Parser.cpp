#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/foreach.hpp>

using namespace std;

class HDB3Parser
{
public:
	//public entry point for encode
	vector<string> Encode(vector<string> data)
	{
		for each (auto item in data)
			this->_convertedData.push_back(EncodeToHDB3(item));

		return this->_convertedData;
	}

	//public entry point for decode
	vector<string> Decode(string readFilename, string writeFilename)
	{
		ReadWrite(readFilename, true);

		for each (auto item in this->_data)
			this->_convertedData.push_back(DecodeFromHDB3(item));

		ReadWrite(writeFilename, false);

		return this->_convertedData;
	}

private:
	vector<string> _data;
	vector<string> _convertedData;

	//reads or writes to a file
	void ReadWrite(string filename, bool read)
	{
		fstream stream;
		string lineIn;
		string temp;

		stream.open(filename);

		if (stream.good())
		{
			if (read)
			{
				cout << "Reading contents of " << filename << "..." << endl;
				while (stream.good())
				{
					getline(stream, lineIn);

					this->_data.push_back(lineIn);
				}
			}
			else
			{
				cout << "Saving data to " << filename << "..." << endl;

				for each (auto str in this->_convertedData)
				{
					if (str != "")
						stream << str << endl;
				}
			}
			stream.close();
		}
		else
			cout << "ERROR: Could not open file: " << filename << endl;
	}

	//encodes given string to HDB3
	string EncodeToHDB3(string item)
	{
		int numZeroes = 0;
		int numPulseCharacters = 0;
		bool prevPulseWasPositive = false;
		bool isAfterFirstViolation = false;
		bool tempPulseBool;
		item = ConvertToAMI(item, false);

		for (int i = 0; i < item.length(); i++)
		{
			if (item[i] == '+')
			{
				prevPulseWasPositive = true;
				numPulseCharacters++;
			}
			else if (item[i] == '-')
			{
				prevPulseWasPositive = false;
				numPulseCharacters++;
			}

			if (item[i] == '0' && item.length() - i >= 4 && item.substr(i, 4) == "0000")
			{
				if (!isAfterFirstViolation)
					isAfterFirstViolation = true;

				if (isOdd(numPulseCharacters))
				{
					if (prevPulseWasPositive)
					{
						item.replace(i, 4, "-00-");
						prevPulseWasPositive = false;
					}
					else
					{
						item.replace(i, 4, "+00+");
						prevPulseWasPositive = true;
					}
					numPulseCharacters = 1;
				}
				else
				{
					if (prevPulseWasPositive)
						item.replace(i, 4, "000+");
					else
						item.replace(i, 4, "000-");

					numPulseCharacters = 1;
				}

				i += 3;
				tempPulseBool = prevPulseWasPositive;
				item = CheckForAMIViolations(item, i, prevPulseWasPositive);

				prevPulseWasPositive = tempPulseBool;
			}
		}
		return item;
	}

	//converts given string to AMI
	string ConvertToAMI(string binaryStr, bool prevPulseWasPositive)
	{
		for (int i = 0; i < binaryStr.length(); i++)
		{
			if (binaryStr[i] == '1' && !prevPulseWasPositive)
			{
				binaryStr[i] = '+';
				prevPulseWasPositive = true;
			}
			else if (binaryStr[i] == '1' && prevPulseWasPositive)
			{
				binaryStr[i] = '-';
				prevPulseWasPositive = false;
			}
		}
		return binaryStr;
	}

	//checks for AMI violations in given string and corrects them
	string CheckForAMIViolations(string item, int currentIndex, bool prevPulseWasPositive)
	{
		int first = currentIndex + 1;
		bool tempPulseBool = prevPulseWasPositive;
		int numAmiViolations = 0;

		for (first; first < item.length(); first++)
		{
			if (prevPulseWasPositive && item[first] == '+')
			{
				item[first] = '-';
				prevPulseWasPositive = false;
				numAmiViolations++;
			}
			else if (!prevPulseWasPositive && item[first] == '-')
			{
				item[first] = '+';
				prevPulseWasPositive = true;
				numAmiViolations++;
			}
			else if (prevPulseWasPositive && item[first] == '-')
				prevPulseWasPositive = false;
			else if (!prevPulseWasPositive && item[first] == '+')
				prevPulseWasPositive = true;
		}
		return item;
	}

	//determines if given number is odd
	bool isOdd(int num)
	{
		if (num % 2 == 1)
			return true;
		return false;
	}

	//decodes given string from HDB3 to binary string
	string DecodeFromHDB3(string item)
	{
		bool prevPulseWasPositive = false;

		for (int i = 0; i < item.length(); i++)
		{
			if (i == 0 && item.length() >= 4 && item.substr(i, 4) == "000+")
			{
				item[i + 3] = '1';
				prevPulseWasPositive = true;
				continue;
			}
			if (item[i] == '+')
			{
				if (item.substr(i, 4) != "+00+")
					item[i] = '1';
				else if (item.length() - i >= 4 && (item[i] == '+' && item.substr(i, 4) == "+00+"))
				{
					item.replace(i, 4, "0000");
					i += 3;
				}
				prevPulseWasPositive = true;
			}
			else if (item[i] == '-')
			{
				if (item.substr(i, 4) != "-00-")
					item[i] = '1';
				else if (item.length() - i >= 4 && (item[i] == '-' && item.substr(i, 4) == "-00-"))
				{
					item.replace(i, 4, "0000");
					i += 3;
				}
				prevPulseWasPositive = false;
			}
			else if (item[i] == '0' && item.length() - i >= 4)
			{
				if (prevPulseWasPositive && item.substr(i, 4) == "000+")
				{
					item.replace(i, 4, "0000");
					i += 3;
					prevPulseWasPositive = true;
				}
				else if (!prevPulseWasPositive && item.substr(i, 4) == "000-")
				{
					item.replace(i, 4, "0000");
					i += 3;
					prevPulseWasPositive = false;
				}
				else if (!prevPulseWasPositive && item.substr(i, 4) == "000+")
				{
					item.replace(i, 4, "0001");
					i += 3;
					prevPulseWasPositive = true;
				}
				else if (prevPulseWasPositive && item.substr(i, 4) == "000-")
				{
					item.replace(i, 4, "0001");
					i += 3;
					prevPulseWasPositive = false;
				}
			}
		}
		return item;
	}
};