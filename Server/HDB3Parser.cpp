#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/foreach.hpp>

using namespace std;

class HDB3Parser
{
public:
	vector<string> Encode(vector<string> data)
	{
		this->_resultData = data;
		EncodeToHDB3();

		return this->_convertedData;
	}

	vector<string> Decode(string readFilename, string writeFilename)
	{
		Read(readFilename);
		DecodeFromHDB3();
		Save(writeFilename, this->_convertedData);
		return this->_resultData;
	}

private:
	vector<string> _resultData;
	vector<string> _convertedData;
	/////////////////////////////////////
	string testEncode;
	string testOrig;
	////////////////////////////////////////
	void Read(string filename)
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

				this->_resultData.push_back(lineIn);
			}
			stream.close();
		}
		else
			cout << "ERROR: Could not open file: " << filename << endl;
	}

	void Save(string filename, vector<string> data)
	{
		ofstream stream;

		stream.open(filename);

		if (stream.good())
		{
			cout << "Saving data to " << filename << "..." << endl;

			for each (auto str in data)
			{
				if (str != "")
					stream << str << endl;
			}
		}
		else
			cerr << "ERROR: Could not open file: " << filename << endl;

		stream.close();
	}

	void EncodeToHDB3()
	{
		int numZeroes = 0;
		int numPulseCharacters = 0;
		bool prevPulseWasPositive = false;


		for each (auto item in this->_resultData)
		{

			//string  item = "0001011000010110000100011010110100100000111101001110010101110011111101001110100101101110011001110010000011101111011101011111010001110000011101011111010001110011";
			//string item = "00010110000101100100000011011001111011110111010111110010001000000111000011110010111011110110011111110010011000010110110100100000011100110110100011101111011101011110110001100100001000000110001011100101001000000110010011100101011101101110010111101100111011110111000011100101011001000010000001110101011100111110100101101110011001110010000011000111110011101101010100100000011101101110010111110010011100111110100111101111011011100111001100100000111011111110011000100000111101000110100011100101001000000100001100101111010000111010101110101011";
			//		string  item = "0001011000010110";
			//testOrig = item;
			//cout << test << endl;
			//test = ConvertToAMI(test);
			item = ConvertToAMI(item);
			//cout << test << endl;
			int first;
			bool isAfterFirstViolation = false;
			//	bool hasOnesViolation = false;
			//int third;


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

				//cout << "i: " << i << endl;
				if (item[i] == '0')
				{
					if (item.length() - i >= 4)
					{
						if (item.substr(i, 4) == "0000") //change this substring to a violation string variable
						{
							if (!isAfterFirstViolation)
							{
								//	numPulseCharacters = 0;
								isAfterFirstViolation = true;
							}
							//cout << test.substr(i, 4) << endl;

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
								{
									item.replace(i, 4, "000+");
								}
								else
								{
									item.replace(i, 4, "000-");
								}

								numPulseCharacters = 1;
							}


							i += 3;
							first = i + 1;

							bool tempPulseBool = prevPulseWasPositive;
							char temp;
							int numAmiViolations = 0;
							for (first; first < item.length(); first++)//flip ones bits if previous substitution pulse is the same as first one bit that follows
							{
								temp = item[first];
								if (prevPulseWasPositive && item[first] == '+')
								{
									item[first] = '-';
									//		cout << "has ones violation" << endl;
									prevPulseWasPositive = false;
									numAmiViolations++;
									//hasOnesViolation = true;
									//			break;
								}
								else if (!prevPulseWasPositive && item[first] == '-')
								{
									item[first] = '+';
									prevPulseWasPositive = true;
									numAmiViolations++;
									//hasOnesViolation = true;
									//	break;
								}
								else if (prevPulseWasPositive && item[first] == '-')
									prevPulseWasPositive = false;
								else if (!prevPulseWasPositive && item[first] == '+')
									prevPulseWasPositive = true;
							}
							prevPulseWasPositive = tempPulseBool;
						}
					}
				}

			}

			this->_convertedData.push_back(item);
		}
		//	testEncode = item;
		//cout << "item is now: " << item << endl;

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
			Look for 4 consecutive zeros and replace them with either 000V or B00V, Rule: If number of + and � at the left hand site of the consecutive 4 zeros is odd then use B00V else 000V
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
		//			  "1010000100001100001110000111100001010000"
		//	string test = "+0-000-+000+-+-00-+-+000+-+-+-00-+0-+00+";

		//			  "1010000011000011000000"
		//string item = "+0-000-0+-+00+-+-00-00";

		//			  "1100000000110000010"
		//string test = "+-000-+00+-+-00-0+0";

		//			  "101000000000010"

		//for each(auto item in this->_resultData)
		//{
		//string item = "+0-000-+00+00-0";
		//bool firstOneFound = false;

		string item = testEncode;
		//		"000+0-+000+-0+-0"
		bool prevPulseWasPositive = false;

		for (int i = 0; i < item.length(); i++)
		{
			if (i == 0 && item.length() >= 4 && item.substr(i, 4) == "000+")
			{
				item[i + 3] = '1';
				prevPulseWasPositive = true;
				//	firstOneFound = true;
				continue;
			}
			if (item[i] == '+')
			{
				if (item.substr(i, 4) != "+00+")
				{
					item[i] = '1';
					prevPulseWasPositive = true;
				}
				else
				{
					if (item.length() - i >= 4 && (item[i] == '+' && item.substr(i, 4) == "+00+"))
					{
						item.replace(i, 4, "0000");
						i += 3;
						prevPulseWasPositive = true;
					}
				}
			}
			else if (item[i] == '-')
			{
				if (item.substr(i, 4) != "-00-")
				{
					item[i] = '1';
					prevPulseWasPositive = false;
				}
				else
				{
					if (item.length() - i >= 4 && (item[i] == '-' && item.substr(i, 4) == "-00-"))
					{
						item.replace(i, 4, "0000");
						i += 3;
						prevPulseWasPositive = false;
					}
				}
			}
			else if (item[i] == '0')//what if string starts with 0?
			{
				if (item.length() - i >= 4 && ((prevPulseWasPositive && item.substr(i, 4) == "000+")))// || (!prevPulseWasPositive && item.substr(i, 4) == "000-")))
				{
					item.replace(i, 4, "0000");
					i += 3;
					prevPulseWasPositive = true;
				}
				else if (item.length() - i >= 4 && ((!prevPulseWasPositive && item.substr(i, 4) == "000-")))// || (!prevPulseWasPositive && item.substr(i, 4) == "000-")))
				{
					item.replace(i, 4, "0000");
					i += 3;
					prevPulseWasPositive = false;
				}
				else if (item.length() - i >= 4 && ((!prevPulseWasPositive && item.substr(i, 4) == "000+")))
				{
					item.replace(i, 4, "0001");
					i += 3;
					prevPulseWasPositive = true;
				}
				else if (item.length() - i >= 4 && ((prevPulseWasPositive && item.substr(i, 4) == "000-")))
				{
					item.replace(i, 4, "0001");
					i += 3;
					prevPulseWasPositive = false;
				}
			}


		}






		//for (int i = 0; i < item.length(); i++)
		//{
		//	if (i == 0 && item.length() >= 4 && item.substr(i, 4) == "000+")
		//	{
		//		item[i + 3] = '1';
		//		//prevPulseWasPositive = true;
		//		//	firstOneFound = true;
		//		continue;
		//	}
		//	if (item[i] == '+' || item[i] == '-')
		//	{
		//		if (item.substr(i, 4) != "+00+" && item.substr(i, 4) != "-00-")
		//			item[i] = '1';
		//		else
		//		{
		//			if (item.length() - i >= 4 && (item[i] == '+' && item.substr(i, 4) == "+00+") || item[i] == '-' && item.substr(i, 4) == "-00-")
		//			{
		//				item.replace(i, 4, "0000");
		//				i += 3;
		//			}


		//		}
		//	}
		//	else if (item[i] == '0')//what if string starts with 0?
		//	{
		//		if (item.length() - i >= 4 && (item.substr(i, 4) == "000+" || item.substr(i, 4) == "000-"))
		//		{
		//			item.replace(i, 4, "0000");
		//			i += 3;
		//		}
		//		else if (item.length() - i >= 4 && ((item[i - 1] == '-' && item.substr(i, 4) == "000+") || (item[i - 1] == '+' && item.substr(i, 4) == "000-")))
		//		{
		//			item.replace(i, 4, "0001");
		//			i += 3;
		//		}
		//	}


		//}





		cout << "item is now: " << item << endl;

		if (testOrig == item)
			cout << "equal" << endl;
		else
			cout << "not equal" << endl;

		this->_convertedData.push_back(item);
		//}
	}
};