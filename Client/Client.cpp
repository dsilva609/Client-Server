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

class Client
{
public:
	//public client entry point
	void Start(string endPoint, string filename)
	{
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

	//creates endpoint for client
	void Create(string endPoint)
	{
		this->_resolver = new tcp::resolver(this->_IOService);
		this->_query = new tcp::resolver::query(endPoint, "daytime");
		this->_endpoint_iterator = (*this->_resolver).resolve(*this->_query);
		this->_socket = new tcp::socket(this->_IOService);
	}

	//receives data from server
	void Read(string filename)
	{
		string receivedData = "";
		vector<string> data;

		cout << "Receiving data..." << endl;
		for (;;)
		{
			boost::array<char, 20000> buf;
			boost::system::error_code error;

			size_t len = (*this->_socket).read_some(boost::asio::buffer(buf), error);

			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

			for (int i = 0; i < len; i++)
				receivedData += buf.at(i);

			data.push_back(receivedData);
			receivedData.clear();
		}
		for each (string str in data)
		{
			cout << "data: " << str << endl;
		}


		SaveToFile(filename, data);

		(*this->_socket).close();
		cout << "Connection to server closed." << endl;
	}

	//saves transmitted data from server to file
	void SaveToFile(string filename, vector<string> receivedData)
	{
		ofstream stream;

		stream.open(filename);

		if (stream.good())
		{
			cout << "Saving data to " << filename << "..." << endl;

			for each (auto str in receivedData)
			{
				if (str != "")
					stream << str << endl;
			}
		}
		else
			cerr << "ERROR: Could not open file: " << filename << endl;

		stream.close();
	}
};
