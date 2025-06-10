
#include <iostream>
#include <boost/json.hpp>
#include <jwt-cpp/jwt.h>
#include <functional>

#include "icontext.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>
namespace json = boost::json;      // from <boost/json.hpp>

using namespace std;

std::string create_jwt( std::string login, std::string password )
{
        auto token_ = jwt::create()
        .set_type("JWS")
        .set_issuer(login)
        .set_payload_claim("password", jwt::claim(password))
        .sign(jwt::algorithm::hs256{"the_holy_grail"});

        return token_;
}

extern "C"  void Handle( IContextPtr context )
{
        http::request<http::string_body> req = context->getRequest();
        if( req.method() != http::verb::post )
        {
                context->setNext(true) ;
                return;
        }

        try{
                json::value _body = json::parse(req.body());

                std::string login = _body.at("login").as_string().c_str();
                std::string password = _body.at("password").as_string().c_str();
                string jwt = create_jwt(login, password);

                http::response<http::string_body> res;
                res.set(http::field::content_type, "application/json");
                json::object json_response;
                json_response["access_token"] = jwt;
                json_response["token_type"] = "Bearer";

                res.body() = json::serialize(json_response);
                res.result(http::status::ok);
                res.set(http::field::content_type, "application/json");
                res.prepare_payload();

                context->setResponse( res );

                context->setNext(false) ;
        }
        catch(exception& e)
        {
                context->setNext(true) ;
        }
}

int main()
{
       std::cout << create_jwt( "std::string login", "std::string password "); 
}