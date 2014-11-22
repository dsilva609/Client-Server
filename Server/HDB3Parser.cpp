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
		int numZeroes = 0;
		int numPulseCharacters = 0;
		bool prevPulseWasPositive = false;
		/*for each  (auto item in this->_resultData)
		{
		cout << item << endl;
		}*/

		string test = "1010000100001100001110000111100001010000";
		cout << test << endl;
		test = ConvertToAMI(test);
		cout << test << endl;
		int first;
		bool hasOnesViolation = false;
		//int third;

		for (int i = 0; i < test.length(); i++)
		{
			if (test[i] == '+')
			{
				prevPulseWasPositive = true;
				numPulseCharacters++;
			}
			else if (test[i] == '-')
			{
				prevPulseWasPositive = false;
				numPulseCharacters++;
			}

			cout << "i: " << i << endl;
			if (test[i] == '0')
			{
				if (test.length() - i >= 4)
				{
					if (test.substr(i, 4) == "0000") //change this substring to a violation string variable
					{
						cout << test.substr(i, 4) << endl;

						if (isOdd(numPulseCharacters))
						{
							if (prevPulseWasPositive)
							{
								test.replace(i, 4, "-00-");
								prevPulseWasPositive = false;
							}
							else
							{
								test.replace(i, 4, "+00+");
								prevPulseWasPositive = true;
							}
							//test.replace(i, 4, "B00V");

							//test.substr(i, 4) = "B00V";
							numPulseCharacters = 1;
						}
						else
						{
							if (prevPulseWasPositive)
							{
								test.replace(i, 4, "000+");//does prevPulseWasPositive need to be updated?
							}
							else
							{
								test.replace(i, 4, "000-");
							}
							//test.substr(i, 4) = "000V";
							numPulseCharacters = 1;
						}


						//cout << "num pulse: " << numPulseCharacters << endl;

						//numPulseCharacters = 0;
						i += 3;
						first = i + 1;
						//third = first + 3;
						bool tempPulseBool = prevPulseWasPositive;

						for (first; first < test.length(); first++)//flip ones bits if previous substitution pulse is the same as first one bit that follows
						{
							if (!hasOnesViolation && prevPulseWasPositive && test[first] == '+')
							{
								//test[first] = '-';
								cout << "has ones violation" << endl;
								hasOnesViolation = true;
								break;
							}
							else if (!hasOnesViolation && !prevPulseWasPositive && test[first] == '-')
							{
								//test[first] = '+';
								hasOnesViolation = true;
								break;
							}
						}
						/*if (hasOnesViolation)
						{
						for (first; first < test.length(); first++)
						{

						if (prevPulseWasPositive && test[first] == '+')
						{
						test[first] = '-';
						prevPulseWasPositive = false;
						}
						else if (!prevPulseWasPositive && test[first] == '-')
						{
						test[first] = '+';
						prevPulseWasPositive = true;
						}

						}
						}*/
						prevPulseWasPositive = tempPulseBool;
					}
				}
			}
		}

		cout << "test is now: " << test << endl;

		/*
			ALGORITHM
			-as the iterator goes through the characters of the string
			---count the number of + and - in the string (pulse counter)
			---determine if that number is odd or even
			---reset both after 4 zeroes are found

			---count the number of zeroes
			---when the number of zeroes reaches 4
			---perform substitutions
			---reset the number after substitution
			---make sure to add the + or - from this substitution to pulse counter

			---run through string again to make sure that there are no duplicate pulses ++ or --

			Algorithm

			Get the signal
			Convert it to Alternate Mark Inversion (AMI) form
			Look for 4 consecutive zeros and replace them with either 000V or B00V, Rule: If number of + and – at the left hand site of the consecutive 4 zeros is odd then use B00V else 000V
			If it is 000V then V polarity should be as same as the polarity of the preceding pulse of those 4 consecutive zeros. If it is B00V, B and V are in same polarity but opposite to the preceding pulse of those 4 consecutive zeros.
			Examples:

			101000000000010

			AMI : +0-0000000000+0

			+0-000V000000+0
			+0-000-B00V00+0
			+0-000-+00+00-0


			*/
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

	bool isOdd(int num)
	{
		if (num % 2 == 1)
			return true;
		return false;
	}

	void DecodeFromHDB3()
	{
		cout << "Does nothing yet" << endl;
	}
};