#ifndef SERVER_HPP
# define SERVER_HPP

#include "utilities_.hpp"
#include "../HTTP/HttpRequest.hpp"
#include "../HTTP/Response.hpp"

class Server;





class	_body
		{
			public :
				_body(int fd)
				{
					_client_fd = fd;
					_body_file.open("body.txt", std::ios::out);
					_body_size = 0;

					_readcount = 0;
					_writecount = 0;
					_red_flag = 0;

					_startedwrite = false;
					_startedread = false;
					_http.set_body_ind(_client_fd);
				}
				~_body()
				{
					_body_file.close();
				}
				HttpRequest			_http;
				std::ostringstream  _body_stream;
				std::fstream    	_body_file;
				size_t          	_body_size;

				Response 			_ok;
				size_t				_readcount;
				size_t				_writecount;

				bool				_startedread;
				bool				_startedwrite;
				int					_red_flag;

				int					_client_fd;
				



				///////ayoub
				std::string response;
				//////



				void				close_file()
				{
					_body_file.close();
				}
				void				init_values(std::string & my_method, std::string & my_chunk, int  index, int	& my_len, std::string & request_target)
				{
					my_method = this->_http.Get_Http_Method();
					my_chunk = this->_http.get_value("Transfer-Encoding");
					my_len = this->_http.get_content_len();
					request_target = this->_http.Get_Request_Target();
					this->_ok.set_my_upload_path(this->_ok.get_server(index).get_upload_path());
					// std::cout << request_target << "HANA ASAT WTF" << std::endl;
					this->_ok.setIndex(index);
					// if (this->_ok.get_server(index).get_redirection_value(request_target) != "")
					// 	this->_red_flag = 1;
					//this->_ok.set_redirect_path(this->_ok.get_server(index).get_redirection_value(request_target));
				}
				// int					get_red_flag()
				// {
				// 	return this->_red_flag;
				// }
				void				set_values(std::string my_method, std::string & error_msg)
				{
					this->_ok.set_request_method(my_method);
					this->_ok.set_request_target(this->_http.Get_Request_Target());
					this->_ok.set_mybuffer(this->_http.Get_Request_Target());
					error_msg = this->_ok.check_my_location(this->_http.Get_Request_Target(), this->_http.Get_Http_Method());
					//error_msg = this->_ok.parsing_check();
				}

				int				handle_body(std::string my_method, std::string my_chunk, std::string error_msg, int my_len)
				{
					this->set_values(my_method, error_msg);
					this->_http.set_my_upload_path(this->_ok.get_my_upload_path());	
					if (my_method == "POST")
					{
						this->_body_file << this->_body_stream.str() << std::endl;
						this->_body_file.close();  
						if (my_chunk == "chunked")
							this->_body_size = this->_http.handle_chunked_body();
						else
							this->_http.handle_regular_body();
						my_len = 0;
						// if (this->_body_size - 1 > my_len && my_len >= 0)
						// 	return 0;
					}
					return 1;
				}

				void				handle_response(std::string my_method)
				{
					if (my_method == "GET")
						this->_body_size = this->_ok.handle_Get_response();
					else if (my_method == "DELETE")
						this->_ok.handle_delete_response(this->_http.get_value("Connection"));
					else if (my_method == "POST")
						this->_ok.handle_post_response(this->_http.get_value("Connection"));
					else
						this->_ok.error_handling("405 Method Not Allowed");
				}


			Server *srvr;
		};


class Server
{

	public:

		int Create();

		int SAcceptCon();


		//NETWORK I/O
		int		accept();
		int		send(int sock, _body * bod,	std::string config);
		int		send(int sock);
		int		recv(int sock);

		//ACCESSORS
		void 	setSocket(std::string host, int port);
		int		getsocketfd();
		void	setAddress();

		void	setName(std::string name);
		std::string	getName();
		
		int			getPort();
		std::string	getHost();

		void		setIndex(int i);
		int			getIndex();

		///////ayoub
		int		CGI_D_ayoub(_body * bd, std::string	request_target, std::string	my_method);
		///////ayoub

		//CONSTRUCTORS AND OVERLOADS
		Server();
		Server(unsigned int host, int port);
		void				set_config(std::string c)
		{
			this->config = c;
		}
		std::string			get_config(void)
		{
			return this->config;
		}
		Server( Server const & src );
		~Server();
		Server &		operator=( Server const & rhs );

	private:
		//std::vector<> of requests
		//std::vector<> of responses

		int							_fd;
		//socket elements
		int							_port;
		unsigned int 				_host;
		std::string					_name;
		std::string 				_hoststring;
		std::string					config;



		struct sockaddr_in			_addr;
		int _addrlen;



		std::vector<HttpRequest>	_requestlist;
		std::map<int, _body *>		_requestmap;




		int					_index;
		

	struct SocketException : public std::exception
	{
   		const char * what () const throw () 
		{
      		return "Error creating server Socket";
   		}
	};

	struct BindException : public std::exception 
	{
   		const char * what () const throw () 
		{
      		return "Error IN Bind";
   		}
	};

	struct ListenException : public std::exception 
	{
   		const char * what () const throw () 
		{
      		return "Error IN listen";
   		}
	};
    
};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */