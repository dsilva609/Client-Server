#include <iostream>
#include <fstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/regex.h>
#include <boost/algorithm/string/regex.hpp>
#include <bitset>
#include <regex>

using boost::asio::ip::tcp;
using namespace std;

class Client {
public:
	void Start(string endPoint, string filename) {
		try
		{
			Create(endPoint);

			boost::asio::connect(*this->_socket, this->_endpoint_iterator);
			cout << "Connected to host at " << endPoint << endl;
			Read(filename);
		}
		catch (exception &e)
		{
			cerr << e.what() << endl;
		}
	}
private:
	boost::asio::io_service _IOService;
	tcp::resolver *_resolver;
	tcp::resolver::query *_query;
	tcp::resolver::iterator _endpoint_iterator;
	tcp::socket *_socket;

	void Create(string endPoint) {
		this->_resolver = new tcp::resolver(this->_IOService);
		this->_query = new tcp::resolver::query(endPoint, "daytime");
		this->_endpoint_iterator = (*this->_resolver).resolve(*this->_query);
		this->_socket = new tcp::socket(this->_IOService);
	}

	void Read(string filename) {
		string receivedData = "";

		cout << "Reveiving data..." << endl;
		for (;;) {
			boost::array<char, 536> buf;
			boost::system::error_code error;

			size_t len = (*this->_socket).read_some(boost::asio::buffer(buf), error);

			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

			for (int i = 0; i < len; i++)
				receivedData += buf.at(i);
		}
		SaveToFile(filename, receivedData);

		(*this->_socket).close();
		cout << "Connection to server closed." << endl;
	}

	void SaveToFile(string filename, string receivedData) {
		ofstream stream;
		vector<string> contents;
		string reg = "";

		reg += bitset<8>(char(22)).to_string();
		reg += reg;


		boost::algorithm::split_regex(contents, receivedData, boost::regex(reg));

		for (int i = 0; i < contents.size(); i++) {
			if (contents.at(i).size() != 0)
				contents.at(i) = reg + contents.at(i);
		}

		stream.open(filename);

		if (stream.good()) {
			cout << "Saving data to " << filename << "..." << endl;

			for each (auto str in contents) {
				if (str != "")
					stream << str << endl;
			}
		}
		else
			cerr << "ERROR: Could not open file: " << filename << endl;

		stream.close();
	}
};