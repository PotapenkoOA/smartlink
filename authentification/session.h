
#ifndef __SESSIONS__
#define __SESSIONS__

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <boost/url.hpp>
#include <iostream>
#include <string>
#include <list>

using namespace boost::urls;
using std::cout, std::endl;

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>
namespace net = boost::asio;       // from <boost/asio.hpp>
namespace json = boost::json;      // from <boost/json.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

#include "context.h"

#include "icommand.h"
#include "strategy.h"


class RoutinHandler{
    tcp::socket m_socket;

    ContextPtr m_context;

    ICommandPtr m_Handler;

    public:
    RoutinHandler(tcp::socket  socket)
    :m_socket(std::move(socket))
    {
        m_context = std::make_shared<Context>();
        m_Handler = make_shared<StrategyCmd>(m_context);
    }

    void Execute()
    {
        beast::flat_buffer buffer;  
        http::read( m_socket, buffer, m_context->req);

        std::cout<<"body:"<<m_context->req<<std::endl;

        m_Handler->Execute();

        http::write( m_socket, m_context->res);
    }
};

class Session
{
    std::unique_ptr<RoutinHandler> routin;
    
    public:
    Session(tcp::socket  socket)   
    {
       routin = std::make_unique<RoutinHandler>(std::move(socket));
    }
    
    // Handle the session in a new thread
    void Start()
    {        
        std::thread{ 
          [&](std::shared_ptr<RoutinHandler> routin)
            {
                routin->Execute();
            }, std::move(routin)
         }.detach();
    }
};


#endif