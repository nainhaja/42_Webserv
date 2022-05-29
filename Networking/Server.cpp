#include "Server.hpp"


// print("Content-type:text\html")
/*
** --------------------------------- METHODS ----------------------------------
*/



int Server::Create()
{


	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0)
		throw SocketException();

	fcntl(_fd, F_SETFL, O_NONBLOCK);

	if (bind(_fd, (struct sockaddr * ) &_addr, sizeof(_addr)) == -1)
		throw BindException();
	
	if (listen(_fd, 1000) < 0)
		throw ListenException();// 1000 10000 EDIT
	std::cout <<  "SERVER FD : " << _fd << std::endl;

	return (0);
}

/*
** --------------------------------- I/O NETWORKING ---------------------------------
*/

int		Server::accept()
{
	//int clnt = accept(_fd, (struct sockaddr * ) &_addr, sizeof(_addr));
	int clnt;
	if ((clnt = ::accept(_fd, (struct sockaddr *)&_addr, (socklen_t*)&_addrlen)) < 0)
    {
		std::cout << "[   ]" <<  _fd << "[   ]"<< "hello from accept" << std::endl;
        perror("In accept");
        exit(EXIT_FAILURE);
    }
	fcntl(clnt, F_SETFL, O_NONBLOCK);

	_requestmap.insert(std::make_pair(clnt, new _body()));

	return (clnt);
}

int		Server::send(int sock)
{
	std::map<int, _body *>::iterator	it;

	it = _requestmap.find(sock);
	if (it == _requestmap.end())
	{
		std::cout << "ERROR IN Request MAP\n";
		exit(EXIT_FAILURE);
	}


	_body *bd;


	bd = it->second;
	std::string	my_method;
	std::string	my_chunk;
	int			my_len;
	std::string	error_msg;
	std::string	request_target;
	std::string	red_target;

	bd->init_values(my_method, my_chunk, _index, my_len, request_target);
	
	if (it->second->_startedwrite == false)
		it->second->_startedwrite = true;
	red_target = request_target.substr(request_target.find_last_of("/") + 1, request_target.size());
	if (bd->_ok.get_server(_index).get_redirection_value(red_target))
	{
		request_target = red_target;
		bd->_ok.set_request_target(request_target);
		bd->_ok.handle_redirect_response(bd->_http.get_value("Connection"));
	}
	else if (my_method == "POST" && ((my_len < 0) || (bd->_body_stream.str() == "" && my_len != 0)))
		bd->_ok.error_handling("400 Bad Request");
	else
	{
		if (!bd->handle_body(my_method, my_chunk, error_msg, my_len))
			bd->_ok.error_handling("400 Bad Request");
		else
		{
			if (bd->_ok.get_max_body_size() < 0)
				bd->_ok.error_handling("500 Webservice currently unavailable");
			else if (bd->_http.get_total_size() > bd->_ok.get_max_body_size() && bd->_ok.get_max_body_size() != 0)
				bd->_ok.error_handling("413 Payload Too Large");
			else
			{
				if (error_msg != "")
					bd->_ok.error_handling(error_msg);
				else
					bd->handle_response(my_method);
				
			}			
		}
	}

	bd->_writecount += write(sock , bd->_ok.get_hello() + bd->_writecount , bd->_ok.get_total_size() - bd->_writecount);
	//bd->_ok.clear();
	//bd->close_file();
	//delete[] bd;
	if (bd->_ok.get_total_size() <= bd->_writecount)
	{
		_requestmap.erase(it);
		return 0;
	}
	else
		return (1);

}
int		Server::recv(int sock)
{
	std::map<int, _body *>::iterator	it;
	int									flag;

	it = _requestmap.find(sock);
	if (it ==  _requestmap.end())
	{
		std::cout << "couldnt receve request\n";
		//exit(EXIT_FAILURE);
	}
	_body *bd = it->second;
	flag = bd->_http.handle_http_request(sock, bd->_body_file, bd->_body_size, bd->_body_stream);
	return flag;
}

/*
** --------------------------------- Modefiers ---------------------------------
*/

int Server::SAcceptCon()
{
	// std::cout << "hello from accept§" << std::endl;
	// //int clnt = accept(_fd, (struct sockaddr * ) &_addr, sizeof(_addr));
	// int clnt;
	// if ((clnt = ::accept(_fd, (struct sockaddr *)&_addr, (socklen_t*)&_addrlen)) < 0)
    // {
    //     perror("In accept");
    //     exit(EXIT_FAILURE);
    // }
	// fcntl(clnt, F_SETFL, O_NONBLOCK);

	// return (clnt);
	return 0;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

	void 	Server::setSocket(std::string hoststring, int port)
	{
		//TODO: change to network/host EQUIVALENT
		_port = port;//htons(port);
		_host = inet_addr(hoststring.c_str());
		_hoststring = hoststring;
		setAddress();
	}

	void	Server::setAddress()
	{
		memset((char *)&_addr, 0, sizeof(_addr)); 
    	_addr.sin_family = AF_INET;
    	_addr.sin_port =  htons(_port);//htons(_port);
		//std::cout << _host << std::endl;
    	_addr.sin_addr.s_addr = _host;//INADDR_ANY;//_host;//htonl(_host);0.0.0.0 127.0.0.1
		_addrlen = sizeof(_addr);
	}

	int		Server::getsocketfd()
	{
		return _fd;
	}



		void		Server::setIndex(int i)
		{
			_index = i;
		}
		int			Server::getIndex()
		{
			return _index;
		}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{

}

Server::Server( const Server & src )
{
}

Server::Server(unsigned int host, int port)
{
	_host = host;
	_port = port;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &				Server::operator=( Server const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}

/* ************************************************************************** */