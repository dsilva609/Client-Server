#include <iostream>
#include <bitset>

using namespace std;

#define CRCANSI 1000000000000101

class CRCEncoder
{
public://change to private

	void Calculate(string messege)
	{
		//messege = "1101010000000000000000";
		messege = "1001";
		bitset<8> msg(messege);
		bitset<16> poly(CRCANSI);

		int result;

		//	result = msg.to_ulong() / poly.to_ulong();

		cout << msg.to_ulong() << endl;
		cout << poly.to_ulong() << endl;
		//cout << result << endl;


	}

};

