#ifndef SERVERS_HPP
#define SERVERS_HPP

#include "../utilities_.hpp"

class Servers
{
    private:
        std::vector<Conf>               server;
        int                             server_block;

    public:
        Servers(void);
        //Servers(std::string File_name);
        std::vector<Conf>               get_server();
        void                            parse_server(std::string name);
        ~Servers(void);
};

#endif 