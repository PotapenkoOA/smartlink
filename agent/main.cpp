#include <boost/json.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>

#include "endpoint.h"

namespace json = boost::json;

std::string game_id;
std::string my_jwt;

// Использование:


int main() {
   // post_json("127.0.0.1", "/", R"({"login":"user","password":"qwerty"})");

    Client client;
    client.connect("127.0.0.1", 8087);
    http::request<http::string_body> req{http::verb::get, "/", 11};
    req.set(http::field::host, "localhost:8080");
    req.set(http::field::user_agent, "Boost.Beast-HTTP-Client");
    http::response<http::dynamic_body> res;
    client.query(req, res);
    
    Client client2;
    client2.connect("127.0.0.1", 8080);
    const std::string target = "/"; const std::string& json=R"({"login":"user","password":"qwerty"})";
    http::request<http::string_body> req2{http::verb::post, target, 11};
    req2.set(http::field::host, "127.0.0.1");
    req2.set(http::field::user_agent, "Boost.Beast");
    req2.set(http::field::content_type, "application/json");
    req2.body() = json;
    req2.prepare_payload();  // Автоматически вычисляет Content-Length   

    client2.query(req2, res);
    std::string jscon_result = beast::buffers_to_string(res.body().data());
    json::value tokens = json::parse(jscon_result) ;
    std::string JWT_ = "..";
    if( tokens.is_object() && tokens.as_object().contains( "access_token" )  )
    {
        JWT_ = tokens.as_object().at("access_token").as_string();
    }
    else return -1;
    client2.disconnect();

    http::request<http::string_body> req3{http::verb::get, "/", 11};
    req3.set(http::field::host, "localhost:8080");
    req3.set(http::field::user_agent, "Boost.Beast-HTTP-Client");
    req3.set(http::field::authorization, "Bearer " + JWT_);
    
    client.query(req3, res);
    client.disconnect();
    return 0;
}