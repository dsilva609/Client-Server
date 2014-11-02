#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

class Server
{
public:
	void Start(string endPoint, string port, vector<string> &data)
	{
		try
		{
			Create(endPoint, port);

			for (;;)
			{
				PollConnections();
				for each (string item in data)
					SendData(item);

				(*this->_socket).close();
				cout << "Connection closed." << endl << endl;
			}
		}
		catch (exception& e)
		{
			cerr << e.what() << endl;
		}
	}

private:
	boost::asio::io_service _IOService;
	tcp::resolver *_resolver;
	tcp::resolver::query*_query;
	tcp::endpoint _endPoint;
	tcp::acceptor *_acceptor;
	tcp::socket *_socket;

	void Create(string endPoint, string port)
	{
		try
		{
			this->_resolver = new tcp::resolver(this->_IOService);
			this->_query = new tcp::resolver::query(endPoint, port);
			this->_endPoint = *(*this->_resolver).resolve(*this->_query);

			cout << "Creating end point to " << this->_endPoint.address().to_string() << "..." << endl;
			cout << "Connecting to port " << this->_endPoint.port() << "..." << endl;

			// acceptor object needs to be created to listen for new connections
			this->_acceptor = new tcp::acceptor(this->_IOService, this->_endPoint);
		}
		catch (exception& e)
		{
			cerr << e.what() << endl;
		}
	}

	void PollConnections()
	{
		// creates a socket
		this->_socket = new tcp::socket(this->_IOService);

		// wait and listen
		cout << "Awaiting Incoming Connections..." << endl;
		(*this->_acceptor).accept((*this->_socket));
		cout << "Client connection found." << endl;
	}

	void SendData(string &data)
	{
		boost::system::error_code ignored_error;

		cout << "Sending data..." << endl;
		cout << "data is: " << data << endl;

		boost::asio::write(*this->_socket, boost::asio::buffer(data), boost::asio::transfer_exactly(data.length()), ignored_error);
	}
};