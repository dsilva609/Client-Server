#include <iostream>
#include <string>
#include <vector>
#include <boost/foreach.hpp>

using namespace std;

class HDB3Parser
{
public:
	vector<string> Encode(vector<string> data)
	{
		this->_resultData = data;
		EncodeToHDB3();

		return this->_resultData;
	}

	vector<string> Decode(vector<string> data)
	{
		this->_resultData = data;
		DecodeFromHDB3();

		return this->_resultData;
	}

private:
	vector<string> _resultData;

	void EncodeToHDB3()
	{
		/*for each  (auto item in this->_resultData)
		{
		cout << item << endl;
		}*/

		string test = "101000000000010";
		cout << test << endl;
		test = ConvertToAMI(test);
		cout << test << endl;
	}

	string ConvertToAMI(string binaryStr)
	{
		bool prevPulseWasPositive = false;

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
			else
				continue;
		}

		return binaryStr;
	}
	void DecodeFromHDB3()
	{
		cout << "Does nothing yet" << endl;
	}
};