#include <iostream>
#include <bitset>

using namespace std;

#define CRCANSI "11000000000000101"

class CRCEncoder
{
public://change to private

	void Calculate(string messege)
	{
		messege = "1101010000000000000000";
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
		bitset <16> num1("010101010010010110");
		bitset <16> num2("100");
		bitset <16> result;
		num1 ^= num2;
		cout << num1;// << endl;
	}

};