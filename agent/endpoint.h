#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>

namespace json = boost::json;
namespace net = boost::asio;       // from <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>


class Client{
    
    net::io_context ioc;

    tcp::socket socket{ioc};
    const std::string host;

    public:
    void connect(uint16_t port)
    {
        try {
            tcp::resolver resolver(ioc);
            
            tcp::endpoint ep = { tcp::v4(), port};
            socket.connect(ep);

        } catch (std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }

    void query(http::request<http::string_body> req, http::response<http::dynamic_body> &res) {
        try {
            http::write(socket, req);
             std::cout<<"Request:\n" << req << std::endl<< std::endl;   

            beast::flat_buffer buffer;
            http::read(socket, buffer, res);

            std::cout<<"Response:\n" << res << std::endl<< std::endl;            

        } catch (std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }

    void disconnect()
    {
        socket.shutdown(tcp::socket::shutdown_both);
    }
    
};






