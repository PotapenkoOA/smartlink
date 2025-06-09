#ifndef __TCP_SERVER__
#define __TCP_SERVER__

#include <boost/asio.hpp>

#include <iostream>

namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

#include "session.h"
#include "iserver.h"

class Server: public IServer
{
    net::io_context ioc;
    std::shared_ptr<tcp::acceptor> acceptor;
    
    public:
    Server( unsigned short port )
    {
        tcp::endpoint ep = { tcp::v4(), port};
        acceptor = std::make_shared<tcp::acceptor>(ioc, ep);

        // Локальный endpoint (после bind/connect)
       
    }

    void Loop()
    {
        while (true) {
            // Accept a new connection
            tcp::socket socket{ioc};
           
            acceptor->accept(socket);

            Session s(std::move(socket));
            s.Start();
        }
    }
};


#endif