#ifndef Response_hpp
#define Response_hpp

#include <stdio.h>
#include <sys/socket.h>
# include <netinet/in.h>
#include <map>
#include <string>
#include <iterator>
#include <vector>
#include "Location.hpp"
#include "Cgi.hpp"
#include "Conf.hpp"
#include "Servers.hpp"


class Response
{
    private:
        std::string                     Display_type;
        int                             status_code;
        std::vector<Conf>               my_servers;      
        int                             pos;  
        std::string                     abs_path;
        std::string                     dir_list;
        std::string                     my_buffer;
        char                            *hello;
        std::ostringstream              my_Res;
        std::string                     dir_body;
        size_t                          body_size;
        size_t                          total_size;
        std::string                     req_method;
        std::string                     req_target;
        std::string                     complete_body;
        int                             my_body_flag;
        int                             max_body_size;
        std::string                     my_upload_path;
        std::string                     redirect_path;
        std::string                     regular_path;
        std::string                     config;
        int                             location_indice;


        int                             _index;
    public:
        Response(void);
        Response(std::string File_name);
        char                        *get_date(void);
        std::string                 parsing_check();
        std::string                 pars_check(std::string target_file, std::string my_method);
        Conf                        get_server(int index);
        std::string                 check_file(void);
        std::string                 check_my_location(std::string request_target, std::string request_method);
        int                         check_my_method(std::string request_method, std::vector <std::string> Allow_methods);
        size_t                      handle_Get_response(void);
        void                        error_handling(std::string error);
        std::string                 get_file_type(std::string type);
        std::string                 get_my_upload_path(void);
        void                        set_my_upload_path(std::string c);
        bool                        check_dir(std::string path);
        std::string                 split_file_path(std::string type);
        void                        set_mybuffer(std::string c);
        std::string                 get_mybuffer(void);
        char*                       get_hello();
        void                        set_hello(std::string c);
        std::string                 genErrorPage(int code);
        size_t                      get_body(std::string path);
        void                        handle_delete_response(std::string connection);
        void                        handle_post_response(std::string connection);
        size_t                      get_body_size(void);
        int                         get_max_body_size(void);
        void                        set_max_body_size(int c);
        size_t                      get_total_size(void);
        std::string                 get_error_body(std::string path);
        void                        set_request_method(std::string c);
        std::string                 get_request_method(void);
        void                        set_request_target(std::string c);
        std::string                 get_request_target(void);
        void                        set_redirect_path(std::string c);
        std::string                 get_redirect_path(void);
        int                         get_body_flag(void);
        void                        clear();
        int                         check_errors();
        void                        initiate_response(std::string & target_file);
        int                         handle_dir(std::string target_file, std::string body, struct stat &status);
        void                        handle_file(struct stat &status);
        std::string                 get_my_res(void);
        int                         handle_dir_response(std::string target_file);
        int                         handle_special_dir(std::string target_file, struct stat &status, std::string & body);
        void                        handle_redirect_response(std::string c);
        int                         check_index_of_dir();
        int                         search_dir_in_locations(std::string pos);
        void                        Edit_path();
        void                        special_dir_treatment();
        void                        special_dir_in_location();
        void                        special_dir_in_server();
        int                         search_index_in_location();
        int                         search_index_in_server();
        void                        dir_treatment();
        void                        arrange_config(std::string c);
        void				        set_config(std::string c);
        std::string			        get_config(void);
        int                         find_location(std::vector <std::string>   location_paths, std::string my_path);
        ~Response(void);



        std::string                 parse_response_cgi(std::string);

        void                         setIndex(int i);
};

#endif 