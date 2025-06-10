
#ifndef __ROUTINCMD__
#define __ROUTINCMD__

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <iostream>
#include <string>
#include <list>

using std::cout, std::endl;

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

#include "icontext.h"
#include "icommand.h"
#include "strategy.h"

class RoutineHandler : public ICommand
{
    tcp::socket m_socket;
    ICommandPtr m_MessageHandler;  

    IContextPtr m_context;

    std::shared_ptr< http::request<http::string_body> > m_req;
    std::shared_ptr< http::response<http::string_body> > m_res;

    public:
    RoutineHandler(tcp::socket  socket)
    :m_socket(std::move(socket))
    {
        m_req = std::make_shared<http::request<http::string_body>>();
        m_res = std::make_shared<http::response<http::string_body>>();
        m_context = std::make_shared<Context>(m_req, m_res);
        m_MessageHandler = make_shared<StrategyCmd>(m_context);
    }

    void Execute()
    {
        while (true)
        {
            beast::flat_buffer buffer;  
            beast::error_code ec;
    
            http::read( m_socket, buffer, *m_req, ec);                        
            if (ec == beast::http::error::end_of_stream) {
                std::cout << "Клиент разорвал соединение\n";
                return;
            } else if (ec) {
                std::cerr << "Ошибка чтения: " << ec.message() << "\n";
                return;
            }

            m_MessageHandler->Execute();

            http::write( m_socket, *m_res);
            m_res->clear();
        }
    }
};

#endif