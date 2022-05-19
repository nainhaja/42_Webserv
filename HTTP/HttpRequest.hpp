#ifndef HttpRequest_hpp
#define HttpRequest_hpp

#include <stdio.h>
#include <sys/socket.h>
# include <netinet/in.h>
#include <map>
#include <string>
#include <iterator>
#include "Location.hpp"

class HttpRequest
{
    private:
        std::string                         Http_Method;
        std::string                         Request_Target;
        std::string                         Protocol_Version;
        std::map<std::string, std::string>  header;
        int                                 fd;
        size_t                              content_length;
        std::string                         tranfer_encoding;
        size_t                              total_size;
    public:
        HttpRequest(void);
        std::map<std::string, std::string>    Parse_Map(std::string buff);
        HttpRequest(std::string buff);

        void            parse_line(std::string str);
        std::string     Get_Http_Method(void);
        std::string     Get_Request_Target(void);
        std::string     Get_Protocol_Version(void);
        void            handle_chunked_body(void);
        void            handle_regular_body(void);
        int             handle_http_request(int new_socket, std::fstream & body_file, size_t &body_size, std::ostringstream & body_stream);
        void            get_request(std::string data, size_t & body_size, std::ostringstream & body_stream);
        void            list_map(void);
        std::string     get_value(std::string key);
        std::string     get_file_type(void);
        void            set_header(std::map<std::string, std::string> c);
        size_t          get_total_size(void);
        ~HttpRequest(void);
};
//body_file
#endif