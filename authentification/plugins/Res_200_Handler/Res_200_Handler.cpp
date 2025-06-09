
#include <iostream>
#include <boost/json.hpp>
#include <jwt-cpp/jwt.h>
#include <functional>

#include "context.h"

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

extern "C"  void Handle( ContextPtr context )
{
        if( context->req.method() != http::verb::post )
        {
                context->next = true;
                return;
        }

        context->next = false;
        
        try{
                json::value _body = json::parse(context->req.body());

                std::string login = _body.at("login").as_string().c_str();
                std::string password = _body.at("password").as_string().c_str();
                string jwt = create_jwt(login, password);


                context->res.set(http::field::content_type, "application/json");
                json::object json_response;
                json_response["access_token"] = jwt;
                json_response["token_type"] = "Bearer";

                context->res.body() = json::serialize(json_response);
                context->res.result(http::status::ok);
                context->res.set(http::field::content_type, "application/json");
                context->res.prepare_payload();

                context->next = false;
        }
        catch(exception& e)
        {
                context->next = true;
        }
}

int main()
{
       std::cout << create_jwt( "std::string login", "std::string password "); 
}