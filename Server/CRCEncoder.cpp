#include <iostream>
#include <bitset>
#include <string>

using namespace std;

#define CRCANSI "11000000000000101"

class CRCEncoder
{
public://change to private

	void Calculate(string messege)
	{
		messege = "1101010000000010111110";
		//messege = "1001";
		/*bitset<8> msg(messege);
		bitset<16> poly(CRCANSI);

		int result;

		result = msg.to_ulong() % poly.to_ulong();

		cout << msg.to_ulong() << endl;
		cout << poly.to_ulong() << endl;
		cout << result << endl;*/

		/*	int num1[] = { 1, 0, 1, 0, 1, 0, 1, 1, 0 };
			int num2[] = { 1, 0, 0 };
			string result = "";

			for (int c = 0; c < 8; c++)
			{
			for (int i = 0; i < 3; i++)
			{
			result += to_string(num1[i] ^ num2[i]);
			}
			cout << result << endl;
			result = "";
			}
			*/
		//bitset<16> num1("010101010010010110");
		//bitset<3> num2("100");
		//string num1 = "10101010010010110";
		string num1 = "1101010000000010111110";
		string num2 = "11000000000000101";
		//bitset<4> result;
		string result = "";

		string current;
		string next;
		//for (int i = 0; i <= num1.length() - num2.length(); i++)
		int i = 0;
		while ((num1.length() >= num2.length()))
		{

			result.clear();//reset at beginning to save value

			current = num1.substr(0, num2.length());

			if (current[0] == '0')
				continue;

			cout << "current dividend: " << current << endl;

			//result.clear();
			for (int j = 0; j < num2.length(); j++)
			{
				//cout << "char " << ((int)(num1[i] - 48)) << endl;

				//result[i] = char(((int)(num1[i] - 48)) ^ ((int)(num2[i] - 48)));

				//cout << result[i] << endl;

				if ((current[j] == '1' && num2[j] == '1') || (current[j] == '0'&& num2[j] == '0'))
					result += '0';
				else
					result += '1';
			}

			next = result + num1.substr(0 + num2.size());//size or length?
			i++;

			cout << "result: " << result << endl;

			for (int k = 0; k < next.length(); k++)
			{
				if (next[k] == '1')
				{
					next = next.substr(k);
					break;
				}
			}
			cout << "current now: " << next << endl;

			num1 = next;


			if (num1.length() < num2.length())
			{
				result = num1;
				break;

			}

			if (bitset <64>(num1).to_ulong() == 0)
			{
				result = "0";
				break;
			}
		}

		cout << "final result: " << result << endl;


		/*   ALGORITHM
		while length of divisor < number

		xor by divisor

		shift to right by 1
		if length is less than divisor starting with first 1
		shift to right again

		*/
	}
};