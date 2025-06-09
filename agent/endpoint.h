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


void post_json(const std::string& host, const std::string& target, const std::string& json) {
    net::io_context io;
    tcp::resolver resolver(io);
    auto const results = resolver.resolve(host, "8080");
    tcp::socket socket(io);
    net::connect(socket, results.begin(), results.end());

    http::request<http::string_body> req{http::verb::post, target, 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, "Boost.Beast");
    req.set(http::field::content_type, "application/json");
    req.body() = json;
    req.prepare_payload();  // Автоматически вычисляет Content-Length

    http::write(socket, req);

    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(socket, buffer, res);

    std::cout << "Response: " << beast::buffers_to_string(res.body().data()) << "\n";
    socket.shutdown(tcp::socket::shutdown_both);
}

class Client{
    // I/O контекст, необходимый для всех I/O операций
    boost::asio::io_context ioc;

    // Resolver для определения endpoint'ов
    //boost::asio::ip::tcp::resolver resolver(ioc);
    // Tcp сокет, использующейся для соединения
    //std::shared_ptr< boost::asio::ip::tcp::socket > socket;
    tcp::socket socket{ioc};
    const std::string host;

    public:
    void connect(const std::string& server_ip, uint16_t port)
    {
        const std::string host = "127.0.0.1";

        try {
    
            // I/O контекст, необходимый для всех I/O операций
            boost::asio::io_context ioc;

            // Resolver для определения endpoint'ов
            boost::asio::ip::tcp::resolver resolver(ioc);
            // Tcp сокет, использующейся для соединения
            //socket = std::make_shared<boost::asio::ip::tcp::socket>(ioc);

            // Резолвим адрес и устанавливаем соединение
            tcp::endpoint ep = { tcp::v4(), port};
            socket.connect(ep);

            std::cout << "Connected to server at " << server_ip << std::endl;
        } catch (std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }

    void query(http::request<http::string_body> req, http::response<http::dynamic_body> &res) {
        try {
           // const std::string target = "/echo?input=test";
            
            // Дальше необходимо создать HTTP GET реквест с указанием таргета
           // http::request<http::string_body> req(http::verb::get, target, 11);
            // Задаём поля HTTP заголовка
            req.set(http::field::host, host);
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
                
            // Отправляем реквест через приконекченный сокет
            http::write(socket, req);

            // Часть, отвечающая за чтение респонса
            {
                boost::beast::flat_buffer buffer;
                //http::response<http::dynamic_body> res;
                http::read(socket, buffer, res);

                std::cout << res << std::endl;
            }

        } catch (std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }

    void disconnect()
    {
        socket.shutdown(tcp::socket::shutdown_both);
    }
    
};






