
#include <iostream>
#include <boost/json.hpp>
#include <boost/algorithm/string.hpp>
#include <jwt-cpp/jwt.h>
#include <functional>

#include "context.h"

using namespace std;

void verify_jwt( string msg )
{
    auto verifier = jwt::verify()
    .allow_algorithm(jwt::algorithm::hs256{"the_holy_grail"});
 
    verifier.verify(jwt::decode(msg));//<<std::endl;
}

std::string extract_bearer_token(const http::request<http::string_body>& req) {
    if (!req.count(http::field::authorization)) {
        return "";
    }

    std::string auth = req[http::field::authorization];
    std::vector<std::string> parts;
    boost::split(parts, auth, boost::is_any_of(" "));

    if (parts.size() != 2 || parts[0] != "Bearer") {
        return ""; // Неверный формат
    }

    return parts[1]; // Возвращаем токен
}

extern "C"  void Handle( ContextPtr context )
{
        if (context->req.find(http::field::authorization) == context->req.end()) {
                context->res.result(http::status::unauthorized);
                context->res.set(http::field::content_type, "text/plain");
                context->res.body() = "Need JWT";
                context->res.prepare_payload();
                context->next = false;
                return;
        }

        try {

               std::string msg = extract_bearer_token( context->req );
               verify_jwt( msg ) ;

               context->res.result(http::status::ok);
               context->next = true;
        }
        catch(...)
        {
                context->res.result(http::status::unauthorized);
                context->res.set(http::field::content_type, "text/plain");
                context->res.body() = "Bad JWT";
                context->res.prepare_payload();
                context->next = false;
                return;
        }
}

int main()
{
       try{ verify_jwt( "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXUyJ9.eyJpc3MiOiJ1c2VyIiwicGFzc3dvcmQiOiJxd2VydHkifQ.pRBnqOCBR9rS6THfb5ixA6WsOZs4D0F39WOqyQMXRHk" ); 
        cout<<"happy end";
       }
        catch(...)
        {
               cout<<"plak";
        }
}